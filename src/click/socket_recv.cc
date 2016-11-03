#include "click/socket_recv.h"
#include <iostream>

#include "zmq.hpp"

#include "zmq_util/zmq_util.h"

namespace latticeflow {

SocketRecv::SocketRecv(const std::string address, zmq::socket_t* socket,
                       Pusher<EnvelopedMessage&&>* downstream)
    : socket_(socket), downstream_(downstream) {
  socket_->bind(address);
}

bool SocketRecv::IsOneOff() const { return false; }

zmq::pollitem_t SocketRecv::GetPollitem() {
  return {*socket_, 0, ZMQ_POLLIN, 0};
}

void SocketRecv::Run(zmq::pollitem_t) {
  downstream_->push(recv_enveloped_msg(socket_));
}

}  // namespace latticeflow
