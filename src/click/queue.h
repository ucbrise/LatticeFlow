#ifndef CLICK_QUEUE_H_
#define CLICK_QUEUE_H_

#include <queue>

#include "boost/optional.hpp"

#include "click/phantoms.h"
#include "click/puller.h"
#include "click/pusher.h"

namespace latticeflow {

template <typename T>
class Queue : public Pusher<T>, Puller<T> {
 public:
  void push(T x) override { xs_.push(std::move(x)); }

  boost::optional<T> pull() override {
    if (xs_.size() == 0) {
      return {};
    } else {
      T x = std::move(xs_.front());
      xs_.pop();
      return x;
    }
  }

 private:
  std::queue<T> xs_;
};

}  // namespace latticeflow

#endif  // CLICK_QUEUE_H_
