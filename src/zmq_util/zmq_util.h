#ifndef ZMQ_UTIL_ZMQ_UTIL_H_
#define ZMQ_UTIL_ZMQ_UTIL_H_

#include <cstring>
#include <ostream>
#include <string>
#include <utility>
#include <vector>

#include <zmq.hpp>

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
  std::vector<zmq::message_t> connection_ids;
  zmq::message_t msg;
};

// Pretty prints a message.
std::ostream& operator<<(std::ostream& out, const zmq::message_t& msg);

// Pretty prints an enveloped message.
std::ostream& operator<<(std::ostream& out, const EnvelopedMessage& e);

// Converts the data within a `zmq::message_t` into a string.
std::string message_to_string(const zmq::message_t& message);

// `send` a string over the socket.
void send_string(const std::string& s, zmq::socket_t* socket);

// `recv` a string over the socket.
std::string recv_string(zmq::socket_t* socket);

// `send` a multipart message.
void send_msgs(std::vector<zmq::message_t>&& msgs, zmq::socket_t* socket);

// `recv` an enveloped message over the socket.
EnvelopedMessage recv_enveloped_msg(zmq::socket_t* socket);

// `send` an enveloped message over the socket.
void send_enveloped_msg(EnvelopedMessage&& s, zmq::socket_t* socket);

// `recv` a multipart message.
std::vector<zmq::message_t> recv_msgs(zmq::socket_t* socket);

// Serialize a proto and `send` it over the socket.
template <typename RequestProto>
void send_proto(const RequestProto& request, zmq::socket_t* socket) {
  std::string request_str;
  request.SerializeToString(&request_str);
  zmq::message_t request_msg(request_str.size());
  memcpy(request_msg.data(), request_str.c_str(), request_str.size());
  socket->send(request_msg);
}

// `recv` a message and unserialize it into a proto.
template <typename ResponseProto>
void recv_proto(ResponseProto* reply, zmq::socket_t* socket) {
  zmq::message_t reply_msg;
  socket->recv(&reply_msg);
  std::string reply_str = message_to_string(reply_msg);
  reply->ParseFromString(reply_str);
}

// `send` a pointer over the socket.
template <typename T>
void send_pointer(const T* const p, zmq::socket_t* socket) {
  zmq::message_t message(sizeof(const T* const));
  memcpy(message.data(), &p, sizeof(const T* const));
  socket->send(message);
}

// `recv` a pointer over the socket.
template <typename T>
T* recv_pointer(zmq::socket_t* socket) {
  zmq::message_t message;
  socket->recv(&message);
  // NOLINT: this code is somewhat forced to be hacky due to the low-level
  // nature of the zeromq API.
  return *reinterpret_cast<T**>(message.data());  // NOLINT
}

// `poll` is a wrapper around `zmq::poll` that takes a vector instead of a
// pointer and a size.
int poll(long timeout, std::vector<zmq::pollitem_t>* items);

}  // namespace latticeflow

#endif  // ZMQ_UTIL_ZMQ_UTIL_H_
