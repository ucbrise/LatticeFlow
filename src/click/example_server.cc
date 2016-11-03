#include <iostream>

#include "zmq.hpp"

#include "click/driver.h"
#include "click/map.h"
#include "click/phantoms.h"
#include "click/socket_recv.h"
#include "click/socket_send.h"
#include "zmq_util/hexdump.h"
#include "zmq_util/zmq_util.h"

namespace lf = latticeflow;

lf::EnvelopedMessage enveloped_id(lf::EnvelopedMessage&& e) {
  std::cout << e << std::endl << std::endl;
  return std::move(e);
}

int main() {
  using EnvMsg = lf::EnvelopedMessage;

  zmq::context_t context(1);
  zmq::socket_t router(context, ZMQ_ROUTER);

  const std::string address = "tcp://*:5555";
  lf::SocketSend out(&router);
  auto print = lf::make_map<EnvMsg, EnvMsg&&>(enveloped_id, &out);
  lf::SocketRecv in(address, &router, &print);

  lf::Driver driver;
  driver.RegisterEventHandler(&in);
  driver.Run();
}
