// Key-value server. See README.md for a high level overview!

#include <atomic>
#include <functional>
#include <iostream>
#include <memory>
#include <set>
#include <string>
#include <thread>
#include <vector>

#include <zmq.hpp>

#include "click/call.h"
#include "click/driver.h"
#include "click/map.h"
#include "click/socket_recv.h"
#include "click/socket_send.h"
#include "concurrency/barrier.h"
#include "key_value_stores/message.pb.h"
#include "key_value_stores/util.h"
#include "key_value_stores/zmq_util.h"
#include "lattices/map_lattice.h"
#include "lattices/max_lattice.h"
#include "lattices/set_union_lattice.h"
#include "lattices/timestamp_lattice.h"
#include "zmq_util/zmq_util.h"

namespace lf = latticeflow;

using CId = lf::ConnectionId;
using EnvMsg = lf::EnvelopedMessage;

// The number of server threads.
constexpr int NUM_THREADS = 4;

// If the total number of updates to the key-value store since the last gossip
// reaches THRESHOLD, then the threads gossip with one another.
constexpr int THRESHOLD = 1;

// Mutex used to allow threads to print to stdout without clobbering each other.
std::mutex stdout_mutex;

// Our key-value store is a map from strings to timestamped strings.
using Timestamp = lf::MaxLattice<int>;
using MaxStringLattice = lf::MaxLattice<std::string>;
using TimestampedStringLattice =
    lf::TimestampLattice<Timestamp, MaxStringLattice>;
using Database = lf::MapLattice<std::string, TimestampedStringLattice>;

// Threads send GossipData messages to one another during gossip.
struct GossipData {
  GossipData() : num_processed(0) {}

  // The set of all database changes since the last gossip. Whenever a thread
  // receives a GossipData, it merges `db` into its local copy of the database.
  Database db;

  // The number of threads that have successfully processed this GossipData.
  std::atomic<int> num_processed;
};

struct State {
  // A server's copy of the key-value store.
  Database kvs;

  // The server's local timestamp, incremented when clients begin transactions.
  int local_timestamp = 0;

  // The number of updates to the key value store since the last gossip.
  int update_counter = 0;

  // The keys that have been updated in the database since the last gossip.
  std::set<std::string> change_set;

  // The unique identifier of a thread.
  int thread_id = 0;

  // The socket to which updates are published.
  zmq::socket_t* publisher;
};

// Given a database (`db`) and set of keys that have been updated in the
// database (`change_set`), send a GossipData message over `publisher`.
void send_gossip(const Database& db, const std::set<std::string>& change_set,
                 zmq::socket_t* publisher) {
  // GossipData messages are shared on the heap. When a thread goes to publish
  // a GossipData message, it allocates in on the heap and sends a pointer to
  // the data to the other threads. Later, the last thread to process the
  // update, makes sure to free the memory.
  //
  // TODO(mwhittaker): Should we just serialize things rather than passing
  // pointers around? We'll have to do that anyway when we shift to
  // inter-server gossip.
  auto* gossip_data = new GossipData;
  for (const std::string& key : change_set) {
    gossip_data->db.put(key, db.get(key));
  }
  send_pointer(gossip_data, publisher);
}

// Receive and process a GossipData message over `subscriber` and update `db`
// accordingly.
void receive_gossip(Database* db, EnvMsg msg) {
  GossipData* gossip_data = lf::message_to_pointer<GossipData>(&msg.msg);
  db->join(gossip_data->db);
  // See `send_gossip`.
  if (gossip_data->num_processed.fetch_add(1) == NUM_THREADS - 1) {
    delete gossip_data;
  }
}

// Processes a request from the user and returns a response that should be sent
// to the user. `thread_id` is the id of the currently running thread; `db` is
// the thread's copy of the database; `update_counter` counts the number of
// database updates; `change_set` records the set of keys that have been
// updated in the database; and `local_timestamp` is the local timestamp of the
// thread.
std::string process_request(const communication::Request& request,
                            State* state) {
  std::cout << "[thread " << state->thread_id
            << "] processing request:" << std::endl
            << request.DebugString() << std::endl;
  communication::Response response;

  switch (request.request_case()) {
    case communication::Request::kBeginTransaction: {
      // TODO(mwhittaker): Why local_timestamp + thread_id?
      response.set_timestamp(std::stoi(std::to_string(state->local_timestamp) +
                                       std::to_string(state->thread_id)));
      response.set_succeed(true);
      state->local_timestamp++;
      break;
    }
    case communication::Request::kGet: {
      const std::unordered_map<std::string, TimestampedStringLattice>& raw_db =
          state->kvs.get();
      if (raw_db.count(request.get().key()) == 0) {
        response.set_succeed(false);
      } else {
        const TimestampedStringLattice& l = state->kvs.get(request.get().key());
        response.set_value(l.value().get());
        response.set_timestamp(l.timestamp().get());
        response.set_succeed(true);
      }
      break;
    }
    case communication::Request::kPut: {
      for (const communication::Request::Put::KeyValuePair& kv_pair :
           request.put().kv_pair()) {
        state->change_set.insert(kv_pair.key());
        TimestampedStringLattice p(
            lf::MaxLattice<int>(request.put().timestamp()),
            lf::MaxLattice<std::string>(kv_pair.value()));
        state->kvs.put(kv_pair.key(), p);
        state->update_counter++;
      }
      response.set_succeed(true);
      break;
    }
    case communication::Request::REQUEST_NOT_SET: {
      response.set_succeed(false);
      break;
    }
  }

  // Gossip to other threads.
  if (state->update_counter >= THRESHOLD && NUM_THREADS != 1) {
    send_gossip(state->kvs, state->change_set, state->publisher);
    state->change_set.clear();
    state->update_counter = 0;
  }

  std::string response_str;
  response.SerializeToString(&response_str);
  return response_str;
}

// The main event loop of each thread. This thread is assigned thread id
// `thread_id`; uses the `all_threads_bound` barrier to ensure all threads have
// established their connections; and uses `context` to form connections to
// clients and other threads.
void worker_routine(const int thread_id, Barrier* all_threads_bound,
                    zmq::context_t* context) {
  // Socket connected to clients (technically, the message broker).
  zmq::socket_t responder(*context, ZMQ_REP);
  const std::string broker_address = "tcp://localhost:5560";
  responder.connect(broker_address);
  {
    std::unique_lock<std::mutex> lock(stdout_mutex);
    std::cout << "[thread " << thread_id << "] connected to " << broker_address
              << std::endl;
  }

  // Socket connected to other threads used for gossiping.
  zmq::socket_t publisher(*context, ZMQ_PUB);
  const std::string publisher_address = "inproc://" + std::to_string(thread_id);
  publisher.bind(publisher_address);
  {
    std::unique_lock<std::mutex> lock(stdout_mutex);
    std::cout << "[thread " << thread_id << "] listening on "
              << publisher_address << std::endl;
  }

  // All threads wait for all other threads to create their publisher socket.
  // TODO(mwhittaker): Do we need to do this? Doesn't ZeroMQ allow us to do
  // this in any order?
  all_threads_bound->enter();

  // Subscribe to other threads.
  zmq::socket_t subscriber(*context, ZMQ_SUB);
  for (int i = 0; i < NUM_THREADS; i++) {
    if (i != thread_id) {
      const std::string subscriber_address = "inproc://" + std::to_string(i);
      subscriber.connect(subscriber_address);
      {
        std::unique_lock<std::mutex> lock(stdout_mutex);
        std::cout << "[thread " << thread_id << "] connected to "
                  << subscriber_address << std::endl;
      }
    }
  }
  const char* filter = "";
  subscriber.setsockopt(ZMQ_SUBSCRIBE, filter, strlen(filter));

  State state;
  state.thread_id = thread_id;
  state.publisher = &publisher;

  lf::Driver driver;

  using namespace std::placeholders;
  lf::SocketSend request_out(&responder);
  auto to_message = lf::make_map<const std::string&>(
      [](const std::string& s) -> EnvMsg {
        return {.cid = CId::Empty(), .msg = lf::string_to_message(s)};
      },
      &request_out);
  auto handle_request = lf::make_map<const communication::Request&>(
      std::bind(process_request, _1, &state), &to_message);
  auto to_request = lf::make_map<EnvMsg&&>(
      [](EnvMsg msg) -> communication::Request {
        communication::Request request;
        lf::message_to_proto<communication::Request>(msg.msg, &request);
        return request;
      },
      &handle_request);
  auto request_in = lf::make_socket_recv(&responder, &to_request);

  auto gossip =
      lf::make_call<EnvMsg&&>(std::bind(receive_gossip, &state.kvs, _1));
  auto gossip_in = lf::make_socket_recv(&subscriber, &gossip);

  driver.RegisterEventHandler(&request_in);
  driver.RegisterEventHandler(&gossip_in);
  driver.Run();
}

int main() {
  Barrier all_threads_bound(NUM_THREADS);
  zmq::context_t context(1);
  std::vector<std::thread> threads;
  for (int thread_id = 0; thread_id < NUM_THREADS; ++thread_id) {
    threads.push_back(
        std::thread(worker_routine, thread_id, &all_threads_bound, &context));
  }
  for (std::thread& thread : threads) {
    thread.join();
  }
}
