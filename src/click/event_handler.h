#ifndef CLICK_EVENT_HANDLER_H_
#define CLICK_EVENT_HANDLER_H_

#include "zmq.hpp"

namespace latticeflow {

// See `driver.h` for relevant documentation. Each `EventHandler` represents an
// event to listen for and a corresponding event handler.
class EventHandler {
 public:
  // If an `EventHandler` is one-off, then after its event triggers, it is not
  // polled for again. For example, waiting for a timer would be a one-off
  // event while waiting for packets from the network would not be.
  virtual bool IsOneOff() const = 0;

  // `GetPollitem()` is the poll item that the `Driver` polls on.
  virtual zmq::pollitem_t GetPollitem() = 0;

  // `Run(pollitem)` runs the event handler with the poll item returned by
  // `GetPollitem` modified by `zmq::poll`. `Run` will only be called if the
  // corresponding event happens. It won't be spuriously called.
  virtual void Run(zmq::pollitem_t pollitem) = 0;
};

}  // namespace latticeflow

#endif  // CLICK_EVENT_HANDLER_H_
