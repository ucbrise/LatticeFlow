#include "zmq_util/zmq_util.h"

#include <iomanip>
#include <ios>

#include "zmq_util/hexdump.h"

namespace latticeflow {

std::ostream& operator<<(std::ostream& out, const zmq::message_t& msg) {
  out << HexDump(reinterpret_cast<const unsigned char*>(msg.data()),
                 msg.size());
  return out;
}

std::ostream& operator<<(std::ostream& out, const EnvelopedMessage& e) {
  for (const zmq::message_t& msg : e.connection_ids) {
    out << msg << std::endl;
  }
  out << e.msg;
  return out;
}

std::string message_to_string(const zmq::message_t& message) {
  return std::string(static_cast<const char*>(message.data()), message.size());
}

void send_string(const std::string& s, zmq::socket_t* socket) {
  zmq::message_t message(s.size());
  memcpy(message.data(), s.c_str(), s.size());
  socket->send(message);
}

std::string recv_string(zmq::socket_t* socket) {
  zmq::message_t message;
  socket->recv(&message);
  return message_to_string(message);
}

void send_msgs(std::vector<zmq::message_t>&& msgs, zmq::socket_t* socket) {
  for (std::size_t i = 0; i < msgs.size(); ++i) {
    socket->send(msgs[i], i == msgs.size() - 1 ? 0 : ZMQ_SNDMORE);
  }
}

std::vector<zmq::message_t> recv_msgs(zmq::socket_t* socket) {
  std::vector<zmq::message_t> msgs;
  int more = true;
  std::size_t more_size = sizeof(more);
  while (more) {
    msgs.emplace_back();
    socket->recv(&msgs.back());
    socket->getsockopt(ZMQ_RCVMORE, static_cast<void*>(&more), &more_size);
  }
  return msgs;
}

EnvelopedMessage recv_enveloped_msg(zmq::socket_t* socket) {
  std::vector<zmq::message_t> msgs = recv_msgs(socket);
  assert(msgs.size() >= 2);
  zmq::message_t msg = std::move(msgs.back());
  msgs.pop_back();
  return {std::move(msgs), std::move(msg)};
}

void send_enveloped_msg(EnvelopedMessage&& e, zmq::socket_t* socket) {
  e.connection_ids.emplace_back(std::move(e.msg));
  send_msgs(std::move(e.connection_ids), socket);
}

int poll(long timeout, std::vector<zmq::pollitem_t>* items) {
  return zmq::poll(items->data(), items->size(), timeout);
}

}  // namespace latticeflow
