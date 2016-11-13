#include "click/socket_send.h"

namespace latticeflow {

SocketSend::SocketSend(zmq::socket_t* socket) : socket_(socket) {}

void SocketSend::push(EnvelopedMessage e) {
  send_enveloped_msg(std::move(e), socket_);
}

}  // namespace latticeflow
