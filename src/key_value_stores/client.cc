#include "key_value_stores/client.h"

#include <iostream>

#include "key_value_stores/message.pb.h"
#include "key_value_stores/util.h"

namespace {

std::string usage() {
  return "usage: \n"
         "  BEGIN TRANSACTION\n"
         "  GET <key>\n"
         "  PUT <key> <value>\n"
         "  END TRANSACTION";
}

}  // namespace

void repl(Client *const client) {
  while (true) {
    std::cout << "> " << std::flush;
    std::string input;
    getline(std::cin, input);
    std::vector<std::string> ss;
    split(input, ' ', &ss);

    if (ss.size() == 2 && ss[0] == "BEGIN" && ss[1] == "TRANSACTION") {
      client->begin();
    } else if (ss.size() == 2 && ss[0] == "GET") {
      client->get(ss[1]);
    } else if (ss.size() == 3 && ss[0] == "PUT") {
      client->put(ss[1], ss[2]);
    } else if (ss.size() == 2 && ss[0] == "END" && ss[1] == "TRANSACTION") {
      client->end();
    } else {
      std::cout << "Invalid command: " << input << std::endl;
      std::cout << usage() << std::endl;
    }
  }
}
