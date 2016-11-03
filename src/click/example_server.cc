#include <iostream>
#include <utility>

#include "zmq.hpp"

#include "click/driver.h"
#include "click/drop.h"
#include "click/map.h"
#include "click/phantoms.h"
#include "click/socket_recv.h"
#include "click/socket_send.h"
#include "zmq_util/hexdump.h"
#include "zmq_util/zmq_util.h"
namespace lf = latticeflow;

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

// std::pair<ConnectionId, EnvelopedMessage> split_and_copy() {}

int main() {
  using EnvMsg = lf::EnvelopedMessage;

  zmq::context_t context(1);
  zmq::socket_t router(context, ZMQ_ROUTER);
  const std::string address = "tcp://*:5555";
  router.bind(address);
  std::cout << "Router listening on '" << address << "'." << std::endl;

  lf::SocketSend out(&router);
  auto val_to_val = lf::make_map<EnvMsg&&, EnvMsg&&>(id_val_to_val, &out);
  auto refref_to_val =
      lf::make_map<EnvMsg&&, EnvMsg&&>(id_refref_to_val, &val_to_val);
  auto refref_to_refref =
      lf::make_map<EnvMsg&&, EnvMsg&&>(id_refref_to_refref, &refref_to_val);
  lf::SocketRecv in(&router, &refref_to_refref);

  lf::Driver driver;
  driver.RegisterEventHandler(&in);
  driver.Run();
}
