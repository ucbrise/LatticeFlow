#ifndef KEY_VALUE_STORES_CLIENT_H_
#define KEY_VALUE_STORES_CLIENT_H_

#include <string>

// A key-value store client. Each client processes four types of requests:
//   1. BEGIN TRANSACTION
//   2. GET <key>
//   3. PUT <key> <value>
//   4. END TRANSACTION
class Client {
 public:
  virtual void begin() = 0;
  virtual void get(const std::string& k) = 0;
  virtual void put(const std::string& k, const std::string& v) = 0;
  virtual void end() = 0;
};

// Parses commands from stdin, calling the methods in `Client` when
// appropriate.
void repl(Client* client);

#endif  // KEY_VALUE_STORES_CLIENT_H_
