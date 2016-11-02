#ifndef CLICK_DROP_H_
#define CLICK_DROP_H_

#include "boost/optional.hpp"

#include "click/phantoms.h"
#include "click/puller.h"
#include "click/pusher.h"

namespace latticeflow {

template <typename Direction, typename T>
class Drop;

template <typename T>
class Drop<Push, T> : public Pusher<T> {
 public:
  void push(T) override {}
};

template <typename T>
class Drop<Pull, T> : public Puller<T> {
 public:
  boost::optional<T> pull() override { return {}; }
};

}  // namespace latticeflow

#endif  // CLICK_DROP_H_
