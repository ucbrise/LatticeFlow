#include "click/driver.h"

#include <cassert>
#include <iterator>

namespace latticeflow {

void Driver::RegisterEventHandler(EventHandler* event_handler) {
  event_handlers_.push_back(event_handler);
  pollitems_.push_back(event_handler->GetPollitem());
}

void Driver::Run() {
  while (true) {
    std::vector<EventHandler*> event_handlers = std::move(event_handlers_);
    std::vector<zmq::pollitem_t> pollitems = std::move(pollitems_);
    event_handlers_.clear();
    pollitems_.clear();

    zmq::poll(pollitems.data(), pollitems.size(), -1);

    auto pollitems_it = std::begin(pollitems);
    auto event_handlers_it = std::begin(event_handlers);
    while (pollitems_it != std::end(pollitems)) {
      assert(event_handlers_it != std::end(event_handlers));

      if (pollitems_it->events & pollitems_it->revents) {
        (*event_handlers_it)->Run(*pollitems_it);
        if ((*event_handlers_it)->IsOneOff()) {
          pollitems_it = pollitems.erase(pollitems_it);
          event_handlers_it = event_handlers.erase(event_handlers_it);
          continue;
        }
      }

      pollitems_it++;
      event_handlers_it++;
    }

    event_handlers_.insert(std::end(event_handlers_),
                           std::begin(event_handlers),
                           std::end(event_handlers));
    pollitems_.insert(std::end(pollitems_), std::begin(pollitems),
                      std::end(pollitems));
  }
}

}  // namespace latticeflow
