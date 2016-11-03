#include "zmq_util/zmq_util.h"

#include <iomanip>
#include <ios>

#include "zmq_util/hexdump.h"
#include "zmq_util/msg_util.h"

namespace latticeflow {

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
  return {ConnectionId(std::move(msgs)), std::move(msg)};
}

void send_enveloped_msg(EnvelopedMessage&& env, zmq::socket_t* socket) {
  for (zmq::message_t& msg : env.cid.connection_ids_) {
    socket->send(msg, ZMQ_SNDMORE);
  }
  socket->send(env.msg);
}

int poll(long timeout, std::vector<zmq::pollitem_t>* items) {
  return zmq::poll(items->data(), items->size(), timeout);
}

}  // namespace latticeflow
