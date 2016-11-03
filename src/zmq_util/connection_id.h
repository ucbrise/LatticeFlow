#ifndef ZMQ_UTIL_CONNECTION_ID_H_
#define ZMQ_UTIL_CONNECTION_ID_H_

#include <ostream>
#include <vector>

#include "zmq.hpp"

namespace latticeflow {

struct EnvelopedMessage;

class ConnectionId {
 public:
  friend std::ostream& operator<<(std::ostream& out, const ConnectionId& cid);
  friend bool operator<(const ConnectionId& lhs, const ConnectionId& rhs);

  friend EnvelopedMessage recv_enveloped_msg(zmq::socket_t* socket);
  friend void send_enveloped_msg(EnvelopedMessage&& s, zmq::socket_t* socket);

 private:
  explicit ConnectionId(std::vector<zmq::message_t>&& connection_ids)
      : connection_ids_(std::move(connection_ids)) {}

  std::vector<zmq::message_t> connection_ids_;
};

}  // namespace latticeflow

#endif  // ZMQ_UTIL_CONNECTION_ID_H_
