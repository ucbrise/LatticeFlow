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
// multipart message. See
// http://zguide.zeromq.org/page:all#The-Extended-Reply-Envelope for more
// information.
struct EnvelopedMessage {
  ConnectionId cid;
  zmq::message_t msg;
};

std::ostream& operator<<(std::ostream& out, const EnvelopedMessage& emsg);
bool operator<(const EnvelopedMessage& lhs, const EnvelopedMessage& rhs);

}  // namespace latticeflow

#endif  // ZMQ_UTIL_ENVELOPED_MESSAGE_H_
