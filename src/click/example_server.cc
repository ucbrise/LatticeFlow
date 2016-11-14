#include <cassert>
#include <chrono>
#include <ios>
#include <iostream>
#include <numeric>
#include <random>
#include <string>
#include <thread>
#include <utility>
#include <vector>

#include "zmq.hpp"

#include "click/call.h"
#include "click/dedup.h"
#include "click/driver.h"
#include "click/drop.h"
#include "click/dup.h"
#include "click/group_by.h"
#include "click/map.h"
#include "click/socket_recv.h"
#include "click/socket_send.h"
#include "click/tee.h"
#include "click/timer.h"
#include "zmq_util/zmq_util.h"

namespace lf = latticeflow;

using CId = lf::ConnectionId;
using EnvMsg = lf::EnvelopedMessage;
using Group = std::pair<CId, std::vector<int>>;

// TODO(mwhittaker): Document.
constexpr int kNumWorkers = 4;

// TODO(mwhittaker): Document.
constexpr std::chrono::nanoseconds kDelay = std::chrono::seconds(5);

// Returns a random integer drawn uniformally in the range [0, high]. `high`
// must be non-negative. Stolen from [1].
// [1]: http://www.cppsamples.com/common-tasks/roll-a-die.html
int roll_die(const int high) {
  assert(high >= 0);
  std::random_device random_device;
  std::mt19937 random_engine{random_device()};
  std::uniform_int_distribution<int> die_distribution(0, high);
  return die_distribution(random_engine);
}

// Pretty prints an `EnvelopedMessage` before returning it.
EnvMsg&& print_envmsg(EnvMsg&& e) {
  std::cout << "zmq::message_t(" << e.msg.size() << ")" << std::endl;
  return std::move(e);
};

// Pretty prints a `std::pair<Cid, int>`.
std::pair<CId, int>&& print_envmsg_group(std::pair<CId, int>&& p) {
  std::cout << p.second << std::endl;
  return std::move(p);
}

// `make_group_evnmsg({cid; msg})` returns `(cid, msg.size() + 1)`.
std::pair<CId, int> make_group_evnmsg(EnvMsg env) {
  int i = std::stoi(lf::message_to_string(std::move(env.msg)));
  return {std::move(env.cid), i};
}

// Project the `cid` field of an `EnvelopedMessage`.
CId extract_envmsg_cid(EnvMsg env) { return std::move(env.cid); }

EnvMsg reduce_group(std::pair<CId, std::vector<int>> group) {
  const std::vector<int>& xs = group.second;
  int sum = std::accumulate(std::begin(xs), std::end(xs), 0);
  EnvMsg envmsg{.cid = std::move(group.first),
                .msg = lf::string_to_message(std::to_string(sum))};
  return envmsg;
}

// TODO(mwhittaker): Document.
void length_plus_i(const int i, zmq::context_t* context) {
  zmq::socket_t source(*context, ZMQ_PULL);
  const std::string source_adress = "tcp://localhost:5556";
  source.connect(source_adress);
  std::cout << "Worker " << i << " connected to '" << source_adress << "'."
            << std::endl;

  zmq::socket_t sink(*context, ZMQ_PUSH);
  const std::string sink_adress = "tcp://localhost:5557";
  sink.connect(sink_adress);
  std::cout << "Worker " << i << " connected to '" << sink_adress << "'."
            << std::endl;

  while (true) {
    EnvMsg env = lf::recv_enveloped_msg(&source);
    std::this_thread::sleep_for(std::chrono::seconds(roll_die(i * 2)));
    const int msg_len = env.msg.size();
    env.msg = lf::string_to_message(std::to_string(msg_len + i));
    lf::send_enveloped_msg(std::move(env), &sink);
  }
}

int main() {
  // Set up sockets.
  zmq::context_t context(1);

  zmq::socket_t router(context, ZMQ_ROUTER);
  const std::string router_address = "tcp://*:5555";
  router.bind(router_address);
  std::cout << "Router listening on '" << router_address << "'." << std::endl;

  zmq::socket_t source(context, ZMQ_PUSH);
  const std::string source_address = "tcp://*:5556";
  source.bind(source_address);
  std::cout << "Source listening on '" << source_address << "'." << std::endl;

  zmq::socket_t sink(context, ZMQ_PULL);
  const std::string sink_address = "tcp://*:5557";
  sink.bind(sink_address);
  std::cout << "Sink listening on '" << sink_address << "'." << std::endl;

  // Create the element graph.
  //
  //             out
  //              |
  //           reduce
  //              |
  //     ______group by________
  //    /         |            \
  //   end       push         end
  //    |         |            |
  //    |         |          dedup
  //    |         |            |
  //    |     form group    get cid
  //    |         |            |
  //    |        copy        copy
  //    |         | ________/
  //    |        tee
  //    |         |
  //  timer  from workers
  //    |
  //   copy   to workers
  //     \       |
  //      \     dup
  //       \   /
  //        tee
  //         |
  //        in
  lf::Driver driver;

  lf::SocketSend out(&router);
  auto reduce = lf::make_map<Group&&>(reduce_group, &out);
  auto group_by = lf::make_group_by<CId, int>(&reduce);

  auto timer = lf::make_timer<CId>(&driver, kDelay, &group_by.end());
  auto extract_cid1 = lf::make_map<EnvMsg&&>(extract_envmsg_cid, &timer);
  auto copy_envmsg1 = lf::make_map<EnvMsg&&>(lf::Clone, &extract_cid1);

  auto print_group =
      lf::make_map<std::pair<CId, int>&&>(print_envmsg_group, &group_by.push());
  auto group_envmsg = lf::make_map<EnvMsg&&>(make_group_evnmsg, &print_group);
  auto copy_envmsg2 = lf::make_map<EnvMsg&&>(lf::Clone, &group_envmsg);

  auto dedup = lf::make_dedup<CId>(&group_by.end(), kNumWorkers);
  auto extract_cid2 = lf::make_map<EnvMsg&&>(extract_envmsg_cid, &dedup);
  auto copy_envmsg3 = lf::make_map<EnvMsg&&>(lf::Clone, &extract_cid2);

  auto from_workers_tee =
      lf::make_tee<EnvMsg&&, EnvMsg&&>({&copy_envmsg2, &copy_envmsg3});
  auto from_workers = lf::make_socket_recv(&sink, &from_workers_tee);
  lf::SocketSend to_workers(&source);
  auto copy_envmsg4 = lf::make_map<EnvMsg&&>(lf::Clone, &to_workers);
  auto dup_envmsg = lf::make_dup<EnvMsg&&>(&copy_envmsg4, kNumWorkers);

  auto in_tee = lf::make_tee<EnvMsg&&, EnvMsg&&>({&copy_envmsg1, &dup_envmsg});
  auto in = lf::make_socket_recv(&router, &in_tee);

  // Start worker threads.
  std::vector<std::thread> workers;
  for (int i = 0; i < kNumWorkers; ++i) {
    workers.push_back(std::thread(length_plus_i, i, &context));
  }

  // Start driver.
  driver.RegisterEventHandler(&from_workers);
  driver.RegisterEventHandler(&in);
  driver.Run();
}
