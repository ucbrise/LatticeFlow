#include <cassert>
#include <chrono>
#include <ios>
#include <iostream>
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
// must be non-negative.
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

std::pair<CId, int>&& print_envmsg_group(std::pair<CId, int>&& p) {
  std::cout << p.second << std::endl;
  return std::move(p);
}

std::pair<CId, int> make_group_evnmsg(EnvMsg env) {
  int i = std::stoi(lf::message_to_string(std::move(env.msg)));
  return {std::move(env.cid), i};
}

CId extract_envmsg_cid(EnvMsg env) { return std::move(env.cid); }

#if 0
void print_int(int x) { std::cout << x << std::endl; };

int get_num(EnvMsg&&) {
  static int i = 0;
  return i++;
}
#endif

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
#if 0
  zmq::socket_t source(context, ZMQ_PUSH);
  const std::string source_adress = "tcp://*:5556";
  source.bind(source_adress);
  std::cout << "Source listening on '" << source_adress << "'." << std::endl;

  zmq::socket_t sink(context, ZMQ_PULL);
  const std::string sink_adress = "tcp://*:5557";
  sink.bind(sink_adress);
  std::cout << "Sink listening on '" << sink_adress << "'." << std::endl;

  lf::SocketSend out(&source);
  lf::Drop<CId&&> drop;
  auto copy_env_f = [](EnvMsg&& env) -> EnvMsg { return lf::Clone(env); };
  auto copy_env = lf::make_map<EnvMsg&&, EnvMsg&&>(copy_env_f, &out);

  auto dup = lf::make_dup(&copy_env, NUM_THREADS);
  auto copy_cid_f = [](EnvMsg&& e) -> CId { return e.cid.Clone(); };
  auto copy_cid = lf::make_map<EnvMsg&&, CId&&>(std::move(copy_cid_f), &drop);
  auto tee = lf::make_tee<EnvMsg&&>({&copy_cid, &dup});
  auto val_to_val = lf::make_map<EnvMsg&&, EnvMsg&&>(id_val_to_val, &tee);
  auto refref_to_val =
      lf::make_map<EnvMsg&&, EnvMsg&&>(id_refref_to_val, &val_to_val);
  auto refref_to_refref =
      lf::make_map<EnvMsg&&, EnvMsg&&>(id_refref_to_refref, &refref_to_val);

  lf::SocketRecv in(&router, &refref_to_refref);
  lf::Drop<EnvMsg&&> drop_drop;
  auto refref_to_refref2 =
      lf::make_map<EnvMsg&&, EnvMsg&&>(id_refref_to_refref, &drop_drop);
  lf::SocketRecv from_workers(&sink, &refref_to_refref2);

  std::vector<std::thread> threads;
  for (int i = 0; i < NUM_THREADS; ++i) {
    threads.push_back(std::thread(double_plus_i, i, &context));
  }

  lf::Driver driver;
  driver.RegisterEventHandler(&in);
  driver.RegisterEventHandler(&from_workers);
  driver.Run();

  for (std::thread& thread : threads) {
    thread.join();
  }
#endif

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
  //             out
  //          to_single message or something
  //        every other by group
  //         group
  //      end    push              end
  //                               dedup
  //             form group        get cid
  //             copy              copy
  //             tee
  //   timer     from workers
  //    |
  //   copy      to workers
  //               dup
  //
  //      tee
  //
  //       in

  lf::Driver driver;

  lf::Drop<EnvMsg&&> drop_envmsg;
  lf::Drop<const Group&> drop_group;
  lf::GroupBy<CId, int, const Group&> group_by(&drop_group);

  lf::Timer<CId, CId&&> timer(&driver, kDelay, &group_by.end());
  lf::Map<EnvMsg&&, CId, decltype(&extract_envmsg_cid)> extract_cid1(
      extract_envmsg_cid, &timer);
  lf::Map<EnvMsg&&, EnvMsg&&, decltype(&lf::Clone)> copy_envmsg1(lf::Clone,
                                                                 &extract_cid1);

  lf::Map<std::pair<CId, int>&&, std::pair<CId, int>&&,
          decltype(&print_envmsg_group)>
      print_group(print_envmsg_group, &group_by.push());
  lf::Map<EnvMsg&&, std::pair<CId, int>&&, decltype(&make_group_evnmsg)>
      group_envmsg(make_group_evnmsg, &print_group);
  lf::Map<EnvMsg&&, EnvMsg&&, decltype(&lf::Clone)> copy_envmsg2(lf::Clone,
                                                                 &group_envmsg);

  lf::Dedup<CId, CId&&> dedup(&group_by.end(), kNumWorkers);
  lf::Map<EnvMsg&&, CId, decltype(&extract_envmsg_cid)> extract_cid2(
      extract_envmsg_cid, &dedup);
  lf::Map<EnvMsg&&, EnvMsg&&, decltype(&lf::Clone)> copy_envmsg3(lf::Clone,
                                                                 &extract_cid2);

  lf::Tee<EnvMsg&&, EnvMsg&&> from_workers_tee({&copy_envmsg2, &copy_envmsg3});
  lf::SocketRecv<EnvMsg&&> from_workers(&sink, &from_workers_tee);
  lf::SocketSend to_workers(&source);
  lf::Map<EnvMsg&&, EnvMsg, decltype(&lf::Clone)> copy_envmsg4(lf::Clone,
                                                               &to_workers);
  lf::Dup<EnvMsg&&, EnvMsg&&> dup_envmsg(&copy_envmsg4, kNumWorkers);

  lf::Tee<EnvMsg&&, EnvMsg&&> in_tee({&copy_envmsg1, &dup_envmsg});
  lf::SocketRecv<EnvMsg&&> in(&router, &in_tee);

  // Start worker threads.
  std::vector<std::thread> workers;
  for (int i = 0; i < kNumWorkers; ++i) {
    workers.push_back(std::thread(length_plus_i, i, &context));
  }

  // Start driver.
  driver.RegisterEventHandler(&from_workers);
  driver.RegisterEventHandler(&in);
  driver.Run();

#if 0
  // Create the element graph.
  //
  //                                 +----------------+
  //                                 | print_int_call |
  //                                 +--------^-------+
  //                                          |
  //                                      +---+---+            +-----+
  //                                      | timer |            | out |
  //                                      +---^---+            +--^--+
  //                                          |                   |
  //   +------+ +-------------------+ +-------+-------+ +------------------+
  //   | drop | | print_envmsg_call | | envmsg_to_int | | print_envmsg_map |
  //   +--^---+ +-------------------+ +--------^------+ +------------------+
  //      |               |                    |                  |
  //      +---------------+----------+---------+------------------+
  //                                 |
  //                              +--+--+
  //                              | tee |
  //                              +--^--+
  //                                 |
  //                              +--+--+
  //                              | in  |
  //                              +-----+
  lf::Drop<EnvMsg&&> drop;

  lf::Call<EnvMsg&&, decltype(&print_envmsg)> print_envmsg_call(print_envmsg);

  lf::Call<const int&, decltype(&print_int)> print_int_call(print_int);
  lf::Timer<int, const int&> timer(
      &driver, std::chrono::nanoseconds(5000000000), &print_int_call);
  lf::Map<EnvMsg&&, int, decltype(&get_num)> envmsg_to_int(get_num, &timer);

  lf::SocketSend out(&router);
  lf::Map<EnvMsg&&, EnvMsg, decltype(&print_envmsg)> print_envmsg_map(
      print_envmsg, &out);

  lf::Tee<EnvMsg&&, EnvMsg&&> tee(
      {&drop, &print_envmsg_call, &envmsg_to_int, &print_envmsg_map});
  lf::SocketRecv<EnvMsg&&> in(&router, &tee);

  auto print_group = [](const std::pair<int, std::vector<int>>& p) {
    std::cout << p.first << ": "
              << "[";
    for (std::size_t i = 0; i < p.second.size(); ++i) {
      if (i != 0) {
        std::cout << ", ";
      }
      std::cout << p.second[i];
    }
    std::cout << "]" << std::endl;
  };
  lf::Call<const std::pair<int, std::vector<int>>&, decltype(print_group)&>
      print_groups(print_group);
  lf::GroupBy<int, int, const std::pair<int, std::vector<int>>&> group_by(
      &print_groups);
  group_by.push().push(std::make_pair(0, 1));
  group_by.push().push(std::make_pair(0, 2));
  group_by.push().push(std::make_pair(0, 3));
  group_by.push().push(std::make_pair(0, 4));
  group_by.end().push(1);
  group_by.end().push(0);

  // Start the driver.
  driver.RegisterEventHandler(&in);
  driver.Run();
#endif
}
