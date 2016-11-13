#include <chrono>
#include <iostream>
#include <thread>
#include <utility>

#include "zmq.hpp"

#include "click/call.h"
#include "click/driver.h"
#include "click/drop.h"
#include "click/map.h"
#include "click/socket_recv.h"
#include "click/socket_send.h"
#include "click/tee.h"
#include "zmq_util/zmq_util.h"

// #include "click/dup.h"
// #include "click/tee.h"
// #include "zmq_util/hexdump.h"

namespace lf = latticeflow;

using EnvMsg = lf::EnvelopedMessage;
using CId = lf::ConnectionId;

#if 0
constexpr int NUM_THREADS = 4;

void double_plus_i(const int i, zmq::context_t* context) {
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
    lf::send_enveloped_msg(std::move(env), &sink);
  }
}

lf::EnvelopedMessage id_val_to_val(lf::EnvelopedMessage e) {
  std::cout << __PRETTY_FUNCTION__ << std::endl;
  std::cout << e << std::endl;
  return e;
}

lf::EnvelopedMessage id_refref_to_val(lf::EnvelopedMessage&& e) {
  std::cout << __PRETTY_FUNCTION__ << std::endl;
  std::cout << e << std::endl;
  return std::move(e);
}

lf::EnvelopedMessage&& id_refref_to_refref(lf::EnvelopedMessage&& e) {
  std::cout << __PRETTY_FUNCTION__ << std::endl;
  std::cout << e << std::endl;
  return std::move(e);
}

#endif

int main() {
  zmq::context_t context(1);

  zmq::socket_t router(context, ZMQ_ROUTER);
  const std::string router_address = "tcp://*:5555";
  router.bind(router_address);
  std::cout << "Router listening on '" << router_address << "'." << std::endl;

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
  auto f = [](EnvMsg && e) -> EnvMsg&& {
    std::cout << e << std::endl << std::endl;
    return std::move(e);
  };

  // Element graph.
  lf::SocketSend out(&router);

  lf::Map<EnvMsg&&, EnvMsg, decltype(f)&> print_map(f, &out);
  lf::Drop<EnvMsg&&> drop;
  lf::Call<EnvMsg&&, decltype(f)&> print_call(f);

  lf::Tee<EnvMsg&&, EnvMsg&&> tee({&drop, &print_call, &print_map});
  lf::SocketRecv<EnvMsg&&> in(&router, &tee);

  // Start the driver.
  lf::Driver driver;
  driver.RegisterEventHandler(&in);
  driver.Run();
}
