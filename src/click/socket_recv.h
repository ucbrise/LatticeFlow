#ifndef CLICK_SOCKET_RECV_H_
#define CLICK_SOCKET_RECV_H_

#include "zmq.hpp"

#include "click/event_handler.h"
#include "click/pushable.h"
#include "zmq_util/enveloped_message.h"
#include "zmq_util/zmq_util.h"

namespace latticeflow {

// A `SocketRecv` polls on data arriving on a socket. When data arrives, it is
// read into an `EnvelopedMessage` and moved to a downstream `Pushable`.
template <typename T>
class SocketRecv : public EventHandler {
 public:
  SocketRecv(zmq::socket_t* socket, Pushable<T>* downstream)
      : socket_(socket), downstream_(downstream) {}

  bool IsOneOff() const override { return false; }

  zmq::pollitem_t GetPollitem() override {
    return {*socket_, 0, ZMQ_POLLIN, 0};
  }

  void Run(zmq::pollitem_t) override {
    downstream_->push(recv_enveloped_msg(socket_));
  }

 private:
  zmq::socket_t* socket_;
  Pushable<T>* downstream_;
};

}  // namespace latticeflow

#endif  // CLICK_SOCKET_RECV_H_
