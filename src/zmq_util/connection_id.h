#ifndef ZMQ_UTIL_CONNECTION_ID_H_
#define ZMQ_UTIL_CONNECTION_ID_H_

#include <ostream>
#include <vector>

#include "zmq.hpp"

namespace latticeflow {

struct EnvelopedMessage;

// Multipart messages in ZeroMQ are often prefixed with a number of connection
// identities. For example, ROUTER sockets internally maintain a map from
// connection id to connection. When it receives a message over a connection,
// it prefixes the received multipart message with the connection id of the
// connection. A ConnectionId represents the connection id prefix.
//
// For example, a connection id prefixed multipart message may look something
// like the following. A `ConnectionId` stores the connection identities and
// empty delimiter in some abstracted form.
//
//           +---+----------+   \
//   frame 1 | 6 | 0x310831 |    |
//           +---+----------+    |
//           +---+----------+    |
//   frame 2 | 6 | 0x913759 |    | connection identity
//           +---+----------+    |
//           +---+----------+    |
//   frame 3 | 6 | 0x138481 |    |
//           +---+----------+   /
//           +---+              \
//   frame 4 | 0 |               | empty delimiter
//           +---+              /
//           +---+------------+ \
//   frame 5 | 8 | "hi world" |  | data frame
//           +---+------------+ /
//
// Note that `ConnectionId`s have no public constructor. They must be
// constructed by calling `recv_enveloped_msg`.
//
// See [1] for more information.
//
// [1]: http://zguide.zeromq.org/page:all#The-Extended-Reply-Envelope
class ConnectionId {
 public:
  // Returns an empty ConnectionId.
  static ConnectionId Empty();

  // Copy a `ConnectionId`. `ConnectionId` doesn't have a copy constructor to
  // avoid accidental copies.
  ConnectionId Clone() const;

  // Pretty print a `ConnectionId`.
  friend std::ostream& operator<<(std::ostream& out, const ConnectionId& cid);

  // Compare a `ConnectionId`. The details are unspecified.
  friend bool operator<(const ConnectionId& lhs, const ConnectionId& rhs);

  // `recv_enveloped_msg` is the only way to construct a `ConnectionId`.
  // `ConnectionId`s can't be used for much besides sending using
  // `send_enveloped_msg`.
  friend EnvelopedMessage recv_enveloped_msg(zmq::socket_t* socket);
  friend void send_enveloped_msg(EnvelopedMessage&& s, zmq::socket_t* socket);

 private:
  // See connection_ids_ documentation below.
  explicit ConnectionId(std::vector<zmq::message_t>&& connection_ids)
      : connection_ids_(std::move(connection_ids)) {}

  // Internally, we store all connection identities and the empty delimiter in
  // a single vector of zeromq messages.
  std::vector<zmq::message_t> connection_ids_;
};

}  // namespace latticeflow

#endif  // ZMQ_UTIL_CONNECTION_ID_H_
