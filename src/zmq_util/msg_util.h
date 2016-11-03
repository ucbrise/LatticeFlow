#ifndef ZMQ_UTIL_MSG_UTIL_H
#define ZMQ_UTIL_MSG_UTIL_H

#include <ostream>

#include "zmq.hpp"

namespace latticeflow {

// Pretty prints a message.
std::ostream& operator<<(std::ostream& out, const zmq::message_t& msg);

// Compares two messages.
bool operator<(const zmq::message_t& lhs, const zmq::message_t& rhs);

}  // namespace latticeflow

#endif  // ZMQ_UTIL_MSG_UTIL_H
