#include "zmq_util/connection_id.h"

#include <algorithm>
#include <functional>
#include <iterator>

#include "zmq_util/msg_util.h"

namespace latticeflow {

std::ostream& operator<<(std::ostream& out, const ConnectionId& cid) {
  for (const zmq::message_t& connection_id : cid.connection_ids_) {
    out << connection_id << std::endl;
  }
  return out;
}

bool operator<(const ConnectionId& lhs, const ConnectionId& rhs) {
  auto comp = [](const zmq::message_t& lhs, const zmq::message_t& rhs) {
    return lhs < rhs;
  };
  return std::lexicographical_compare(
      lhs.connection_ids_.cbegin(), lhs.connection_ids_.cbegin(),
      rhs.connection_ids_.cend(), rhs.connection_ids_.cend(), comp);
}

}  // namespace latticeflow
