#include "zmq_util/enveloped_message.h"

#include "zmq_util/msg_util.h"

namespace latticeflow {

EnvelopedMessage Clone(const EnvelopedMessage& env) {
  zmq::message_t clone;
  clone.copy(&env.msg);
  return {env.cid.Clone(), std::move(clone)};
}

std::ostream& operator<<(std::ostream& out, const EnvelopedMessage& emsg) {
  out << "Envelope" << std::endl
      << "========" << std::endl
      << emsg.cid << std::endl
      << "Message" << std::endl
      << "=======" << std::endl
      << emsg.msg;
  return out;
}

bool operator<(const EnvelopedMessage& lhs, const EnvelopedMessage& rhs) {
  return lhs.cid < rhs.cid && lhs.msg < rhs.msg;
}

}  // namespace  latticeflow
