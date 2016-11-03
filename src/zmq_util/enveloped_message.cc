#include "zmq_util/enveloped_message.h"

#include "zmq_util/msg_util.h"

namespace latticeflow {

std::ostream& operator<<(std::ostream& out, const EnvelopedMessage& emsg) {
  out << emsg.cid << std::endl << emsg.msg;
  return out;
}

bool operator<(const EnvelopedMessage& lhs, const EnvelopedMessage& rhs) {
  return lhs.cid < rhs.cid && lhs.msg < rhs.msg;
}

}  // namespace  latticeflow
