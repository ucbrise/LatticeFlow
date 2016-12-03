#ifndef ZMQ_UTIL_ZMQ_UTIL_H_
#define ZMQ_UTIL_ZMQ_UTIL_H_

#include <cstring>
#include <ostream>
#include <string>
#include <utility>
#include <vector>

#include <zmq.hpp>

#include "zmq_util/enveloped_message.h"

namespace latticeflow {

// Converts the data within a `zmq::message_t` into a string.
std::string message_to_string(const zmq::message_t& message);

// Converts a string into a `zmq::message_t`.
zmq::message_t string_to_message(const std::string& s);

// `send` a string over the socket.
void send_string(const std::string& s, zmq::socket_t* socket);

// `recv` a string over the socket.
std::string recv_string(zmq::socket_t* socket);

// `send` a multipart message.
void send_msgs(std::vector<zmq::message_t>&& msgs, zmq::socket_t* socket);

// `recv` an enveloped message over the socket.
EnvelopedMessage recv_enveloped_msg(zmq::socket_t* socket);

// `send` an enveloped message over the socket.
void send_enveloped_msg(EnvelopedMessage&& emsg, zmq::socket_t* socket);

// `recv` a multipart message.
std::vector<zmq::message_t> recv_msgs(zmq::socket_t* socket);

// Wraps a proto into a message.
template <typename Proto>
zmq::message_t proto_to_message(const Proto& proto) {
  std::string s;
  proto.SerializeToString(&s);
  zmq::message_t msg(s.size());
  memcpy(msg.data(), s.c_str(), s.size());
  return msg;
}

// Unwraps a message into a proto.
template <typename Proto>
void message_to_proto(const zmq::message_t& msg, Proto* p) {
  std::string s = message_to_string(msg);
  p->ParseFromString(s);
}

// Serialize a proto and `send` it over the socket.
template <typename RequestProto>
void send_proto(const RequestProto& request, zmq::socket_t* socket) {
  socket->send(proto_to_message(request));
}

// `recv` a message and unserialize it into a proto.
template <typename ResponseProto>
void recv_proto(ResponseProto* reply, zmq::socket_t* socket) {
  zmq::message_t reply_msg;
  socket->recv(&reply_msg);
  message_to_proto<ResponseProto>(reply_msg, reply);
}

// Wraps a pointer into a message.
template <typename T>
zmq::message_t pointer_to_message(const T* const p) {
  zmq::message_t msg(sizeof(const T* const));
  memcpy(msg.data(), &p, sizeof(const T* const));
}

// Unwraps a message into a pointer.
template <typename T>
T* message_to_pointer(zmq::message_t* msg) {
  // NOLINT: this code is somewhat forced to be hacky due to the low-level
  // nature of the zeromq API.
  return *reinterpret_cast<T**>(msg->data());  // NOLINT
}

// `send` a pointer over the socket.
template <typename T>
void send_pointer(const T* const p, zmq::socket_t* socket) {
  socket->send(pointer_to_message(p));
}

// `recv` a pointer over the socket.
template <typename T>
T* recv_pointer(zmq::socket_t* socket) {
  zmq::message_t msg;
  socket->recv(&msg);
  return message_to_pointer<T>(msg);
}

// `poll` is a wrapper around `zmq::poll` that takes a vector instead of a
// pointer and a size.
int poll(long timeout, std::vector<zmq::pollitem_t>* items);

}  // namespace latticeflow

#endif  // ZMQ_UTIL_ZMQ_UTIL_H_
