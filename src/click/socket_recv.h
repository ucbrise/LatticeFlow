#ifndef CLICK_SOCKET_RECV_H_
#define CLICK_SOCKET_RECV_H_

#include "click/event_handler.h"
#include "click/pusher.h"
#include "zmq_util/zmq_util.h"

namespace latticeflow {

class SocketRecv : public EventHandler {
 public:
  SocketRecv(const std::string address, zmq::socket_t* socket,
             Pusher<EnvelopedMessage&&>* downstream);

  bool IsOneOff() const override;
  zmq::pollitem_t GetPollitem() override;
  void Run(zmq::pollitem_t pollitem) override;

 private:
  zmq::socket_t* socket_;
  Pusher<EnvelopedMessage&&>* downstream_;
};

}  // namespace latticeflow
#endif  // CLICK_SOCKET_RECV_H_
