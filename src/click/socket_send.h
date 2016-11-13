#ifndef CLICK_SOCKET_SEND_H_
#define CLICK_SOCKET_SEND_H_

#include <iostream>

#include "zmq.hpp"

#include "click/pushable.h"
#include "zmq_util/zmq_util.h"

namespace latticeflow {

// A `SocketSend` receives, and takes ownership of, packets and sends them out
// on a socket.
class SocketSend : public Pushable<EnvelopedMessage> {
 public:
  explicit SocketSend(zmq::socket_t* socket);

  void push(EnvelopedMessage e) override;

 private:
  zmq::socket_t* socket_;
};

}  // namespace latticeflow
#endif  //  CLICK_SOCKET_SEND_H_
