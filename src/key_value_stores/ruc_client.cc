// READ UNCOMMITTED key-value client.
//
// In Highly Available Transactions: Virtues and Limitations [1], Bailis et al.
// explain that read uncommitted transactions can be achieved by annotating
// each transaction with a globally unique id and enforcing a last-write-wins
// policy on the servers.
//
// [1]: http://www.bailis.org/papers/hat-vldb2014.pdf

#include <iostream>
#include <string>
#include <vector>

#include <zmq.hpp>

#include "key_value_stores/client.h"
#include "key_value_stores/message.pb.h"
#include "key_value_stores/util.h"
#include "key_value_stores/zmq_util.h"

class RucClient : public Client {
 public:
  explicit RucClient(zmq::socket_t* socket)
      : socket_(socket), current_timestamp_(-1) {}

  void begin() override {
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

  void get(const std::string& k) override {
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

  void put(const std::string& k, const std::string& v) override {
    if (current_timestamp_ == -1) {
      std::cout << "First run BEGIN TRANSACTION." << std::endl;
    } else {
      communication::Request request;
      request.mutable_put()->set_timestamp(current_timestamp_);
      communication::Request::Put::KeyValuePair* kv_pair =
          request.mutable_put()->add_kv_pair();
      kv_pair->set_key(k);
      kv_pair->set_value(v);
      request.mutable_put()->set_timestamp(current_timestamp_);
      send_proto(request, socket_);

      communication::Response response;
      recv_proto(&response, socket_);
      if (!response.succeed()) {
        std::cout << "ERROR" << std::endl;
      }
    }
  }

  void end() override {
    std::cout << "END TRANSACTION not supported" << std::endl;
  }

 private:
  zmq::socket_t* const socket_;
  int current_timestamp_;
};

int main() {
  zmq::context_t context(1);
  zmq::socket_t socket(context, ZMQ_REQ);
  socket.connect("tcp://localhost:5559");
  std::cout << "client connected to "
            << "tcp://localhost:5559" << std::endl;

  RucClient client(&socket);
  repl(&client);
}
