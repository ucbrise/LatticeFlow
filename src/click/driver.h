#ifndef CLICK_DRIVER_H_
#define CLICK_DRIVER_H_

#include <vector>

#include "click/event_handler.h"

namespace latticeflow {

// TODO(mwhittaker): Document.
class Driver {
 public:
  void RegisterEventHandler(EventHandler* event_handler);
  void Run();

 private:
  std::vector<EventHandler*> event_handlers_;
  std::vector<zmq::pollitem_t> pollitems_;
};

}  // namespace latticeflow

#endif  // CLICK_DRIVER_H_
