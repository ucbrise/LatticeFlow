#ifndef PUSHPULL_ID_H_
#define PUSHPULL_ID_H_

#include "boost/optional.hpp"

#include "pushpull/phantoms.h"
#include "pushpull/puller.h"
#include "pushpull/pusher.h"

namespace latticeflow {

template <typename Flow, typename T>
class Id;

template <typename T>
class Id<Push, T> : public Pusher<T> {
 public:
  explicit Id(Pusher<T>* downstream) : downstream_(downstream) {}

  void push(T x) override { downstream_->push(x); }

 private:
  Pusher<T>* downstream_;
};

template <typename T>
Id<Push, T> make_id(Pusher<T>* downstream) {
  return Id<Push, T>(downstream);
}

template <typename T>
class Id<Pull, T> : public Puller<T> {
 public:
  explicit Id(Puller<T>* upstream) : upstream_(upstream) {}

  boost::optional<T> pull() override { return upstream_->pull(); }

 private:
  Puller<T>* upstream_;
};

template <typename T>
Id<Pull, T> make_id(Puller<T>* upstream) {
  return Id<Pull, T>(upstream);
}

}  // namespace latticeflow

#endif  // PUSHPULL_ID_H_
