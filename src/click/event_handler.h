#ifndef CLICK_EVENT_HANDLER_H_
#define CLICK_EVENT_HANDLER_H_

#include "zmq.hpp"

namespace latticeflow {

// TODO(mwhittaker): Document.
class EventHandler {
 public:
  virtual bool IsOneOff() const = 0;
  virtual zmq::pollitem_t GetPollitem() = 0;
  virtual void Run(zmq::pollitem_t pollitem) = 0;
};

}  // namespace latticeflow

#endif  // CLICK_EVENT_HANDLER_H_
