#ifndef PUSHPULL_DROP_H_
#define PUSHPULL_DROP_H_

#include "boost/optional.hpp"

#include "pushpull/phantoms.h"
#include "pushpull/puller.h"
#include "pushpull/pusher.h"

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

#endif  // PUSHPULL_DROP_H_
