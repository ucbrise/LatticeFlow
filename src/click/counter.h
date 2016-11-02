#ifndef CLICK_COUNTER_H_
#define CLICK_COUNTER_H_

#include "boost/optional.hpp"

#include "click/phantoms.h"
#include "click/puller.h"
#include "click/pusher.h"

namespace latticeflow {

template <typename Direction, typename T>
class Counter;

template <typename T>
class Counter<Push, T> : public Pusher<T> {
 public:
  explicit Counter(Pusher<T>* downstream) : downstream_(downstream) {}

  void push(T x) override {
    count_++;
    downstream_->push(x);
  }

  int get() { return count_; }

 private:
  Pusher<T>* downstream_;
  int count_ = 0;
};

template <typename T>
Counter<Push, T> make_counter(Pusher<T>* downstream) {
  return Counter<Push, T>(downstream);
}

template <typename T>
class Counter<Pull, T> : public Puller<T> {
 public:
  explicit Counter(Puller<T>* upstream) : upstream_(upstream) {}

  boost::optional<T> pull() override {
    count_++;
    return upstream_->pull();
  }

  int get() { return count_; }

 private:
  Puller<T>* upstream_;
  int count_ = 0;
};

template <typename T>
Counter<Pull, T> make_counter(Puller<T>* upstream) {
  return Counter<Pull, T>(upstream);
}

}  // namespace latticeflow

#endif  // CLICK_COUNTER_H_
