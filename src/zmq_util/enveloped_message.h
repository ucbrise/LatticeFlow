#ifndef ZMQ_UTIL_ENVELOPED_MESSAGE_H_
#define ZMQ_UTIL_ENVELOPED_MESSAGE_H_

#include <ostream>

#include "zmq.hpp"

#include "zmq_util/connection_id.h"

namespace latticeflow {

// Multipart messages in ZeroMQ are often prefixed with a number of connection
// identities. For example, ROUTER sockets internally maintain a map from
// connection id to connection. When it receives a message over a connection,
// it prefixes the received multipart message with the connection id of the
// connection. An EnvelopedMessage represents an connection id prefixed
// multipart message.
//
// For example, an EnvelopedMessage message may looks something like the
// following where `cid` stores some representation of the connection identity
// and empty delimiter, and `msg` stores the data frame.
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
// See [1] for more information.
//
// [1]: http://zguide.zeromq.org/page:all#The-Extended-Reply-Envelope
struct EnvelopedMessage {
  ConnectionId cid;
  zmq::message_t msg;
};

// Copy `env`. `EnvelopedMessage` doesn't have a copy constructor to avoid
// accidental copying of data.
EnvelopedMessage Clone(const EnvelopedMessage& env);

// Pretty print an `EnvelopedMessage`.
std::ostream& operator<<(std::ostream& out, const EnvelopedMessage& emsg);

// Compare two `EnvelopedMessage`s. The comparison is a pairwise comparison of
// `cid` and `msg`.
bool operator<(const EnvelopedMessage& lhs, const EnvelopedMessage& rhs);

}  // namespace latticeflow

#endif  // ZMQ_UTIL_ENVELOPED_MESSAGE_H_
