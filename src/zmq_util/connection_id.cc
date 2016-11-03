#include "zmq_util/connection_id.h"

#include <algorithm>
#include <functional>
#include <iterator>

#include "zmq_util/msg_util.h"

namespace latticeflow {

ConnectionId ConnectionId::Clone() {
  std::vector<zmq::message_t> clone(connection_ids_.size());
  for (std::size_t i = 0; i < clone.size(); ++i) {
    connection_ids_[i].copy(&clone[i]);
  }
  return ConnectionId(std::move(clone));
}

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
