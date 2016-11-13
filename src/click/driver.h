#ifndef CLICK_DRIVER_H_
#define CLICK_DRIVER_H_

#include <vector>

#include "click/event_handler.h"

namespace latticeflow {

// Kohler et al. recognized that routers were increasingly expected to do more
// than route. However, traditional routers were inflexible, and it was hard to
// modify them to meet the increasingly complex functionality demanded of them.
// To solve this problem, they introduced the Click modular router [1]: a
// framework which enables the simple and modular construction of complex
// routers.
//
// Using Click, routers are a directed graph of processing units called
// *elements*. Click routers are 100% event driven. There is a single process;
// there is a single thread; there are no synchronous I/O calls. The leaves of
// the element graph register events and event handlers with an event loop
// which polls asynchronous I/O calls and dispatches to the correct event
// handler which pushes data through the element graph.
//
// A `Driver` implements a main event loop in this style. Code can register
// `EventHandler`s with a `Driver`. Once `Driver::Run` is called, the `Driver`
// begins to poll and dispatch to the correct `EventHandler`s. Moreover,
// `EventHandler`s can be registered with a `Driver` even after `Run` has been
// called.
//
// [1]: http://bit.ly/2esyPOk
class Driver {
 public:
  // Register an `EventHandler` with a `Driver`. This can be called before or
  // after `Run`. The `Driver` does not take ownership of the pointer.
  void RegisterEventHandler(EventHandler* event_handler);

  // Start polling and dispatching to `EventHandler`s. Calling `Run` will block
  // forever.
  void Run();

 private:
  // The `EventHandler`s registered with `RegisterEventHandler`. The pointers
  // are not owned.
  std::vector<EventHandler*> event_handlers_;

  // `pollitems_[i]` is the poll item for `event_handlers_[i]`
  // `event_handlers_` and `pollitems_` are always the same length.
  std::vector<zmq::pollitem_t> pollitems_;
};

}  // namespace latticeflow

#endif  // CLICK_DRIVER_H_
