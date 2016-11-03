#include "zmq_util/msg_util.h"

#include <algorithm>

#include "zmq_util/hexdump.h"

namespace latticeflow {

std::ostream& operator<<(std::ostream& out, const zmq::message_t& msg) {
  out << HexDump(reinterpret_cast<const unsigned char*>(msg.data()),
                 msg.size());
  return out;
}

bool operator<(const zmq::message_t& lhs, const zmq::message_t& rhs) {
  const unsigned char* const lhs_begin =
      reinterpret_cast<const unsigned char* const>(lhs.data());
  const unsigned char* const lhs_end = lhs_begin + lhs.size();

  const unsigned char* const rhs_begin =
      reinterpret_cast<const unsigned char* const>(rhs.data());
  const unsigned char* const rhs_end = rhs_begin + rhs.size();

  return std::lexicographical_compare(lhs_begin, lhs_end, rhs_begin, rhs_end);
}

}  // namespace latticeflow
