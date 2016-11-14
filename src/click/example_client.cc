#include <iostream>
#include <vector>

#include "zmq.hpp"

#include "zmq_util/zmq_util.h"

namespace lf = latticeflow;

int main() {
  zmq::context_t context(1);
  zmq::socket_t socket(context, ZMQ_DEALER);
  const std::string address = "tcp://localhost:5555";
  socket.connect(address);
  std::cout << "Client connected to '" << address << "'." << std::endl;

  std::vector<zmq::pollitem_t> pollitems = {
      {socket, 0, ZMQ_POLLIN, 0}, {nullptr, 0, ZMQ_POLLIN, 0},
  };

  std::string line;
  std::cout << "> " << std::flush;
  while (true) {
    lf::poll(-1, &pollitems);

    if (pollitems[0].revents & ZMQ_POLLIN) {
      std::string msg = lf::recv_string(&socket);
      std::cout << "Received '" << msg << "'." << std::endl;
      std::cout << "> " << std::flush;
    }

    if (pollitems[1].revents & ZMQ_POLLIN) {
      std::getline(std::cin, line);
      lf::send_string(line, &socket);
      std::cout << "Sent '" << line << "'." << std::endl;
      std::cout << "> " << std::flush;
    }
  }
}
