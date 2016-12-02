// READ COMMITTED key-value client.
//
// In Highly Available Transactions: Virtues and Limitations [1], Bailis et al.
// explain that read committed transactions can be achieved by having servers
// simply not write any uncommitted modifications to readable state. The
// simplest way to achieve this is to have a client buffer its updates locally
// before sending them in a single batch to the server.
//
// [1]: http://www.bailis.org/papers/hat-vldb2014.pdf

#include <iostream>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include <zmq.hpp>

#include "key_value_stores/client.h"
#include "key_value_stores/message.pb.h"
#include "key_value_stores/util.h"
#include "key_value_stores/zmq_util.h"

class RcClient : public Client {
 public:
  explicit RcClient(zmq::socket_t *socket)
      : socket_(socket), current_timestamp_(-1) {}

  void begin() {
    communication::Request request;
    request.mutable_begin_transaction();
    send_proto(request, socket_);

    communication::Response response;
    recv_proto(&response, socket_);
    if (response.succeed()) {
      current_timestamp_ = response.timestamp();
      std::cout << current_timestamp_ << std::endl;
    } else {
      std::cout << "ERROR" << std::endl;
    }
  }

  void get(const std::string &k) {
    if (write_buffer_.count(k) != 0) {
      std::cout << write_buffer_[k] << std::endl;
    } else {
      communication::Request request;
      request.mutable_get()->set_key(k);
      send_proto(request, socket_);

      communication::Response response;
      recv_proto(&response, socket_);
      if (response.succeed()) {
        std::cout << response.value() << std::endl;
      } else {
        std::cout << "ERROR" << std::endl;
      }
    }
  }

  void put(const std::string &k, const std::string &v) {
    if (current_timestamp_ == -1) {
      std::cout << "First run BEGIN TRANSACTION." << std::endl;
    } else {
      write_buffer_[k] = v;
    }
  }

  void end() {
    communication::Request request;
    request.mutable_put()->set_timestamp(current_timestamp_);
    for (const std::pair<std::string, std::string> &kv : write_buffer_) {
      communication::Request::Put::KeyValuePair *p =
          request.mutable_put()->add_kv_pair();
      p->set_key(std::get<0>(kv));
      p->set_value(std::get<1>(kv));
    }
    send_proto(request, socket_);

    communication::Response response;
    recv_proto(&response, socket_);
    if (!response.succeed()) {
      std::cout << "ERROR" << std::endl;
    }
    write_buffer_.clear();
  }

 private:
  zmq::socket_t *const socket_;
  int current_timestamp_;
  std::unordered_map<std::string, std::string> write_buffer_;
};

int main() {
  zmq::context_t context(1);
  zmq::socket_t socket(context, ZMQ_REQ);
  socket.connect("tcp://localhost:5559");
  std::cout << "client connected to "
            << "tcp://localhost:5559" << std::endl;

  RcClient client(&socket);
  repl(&client);
}
