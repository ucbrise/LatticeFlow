#ifndef PUSHPULL_FILTER_H_
#define PUSHPULL_FILTER_H_

#include <functional>

#include "boost/optional.hpp"

#include "pushpull/phantoms.h"
#include "pushpull/puller.h"
#include "pushpull/pusher.h"

namespace latticeflow {

template <typename Direction, typename T>
class Filter;

template <typename T>
class Filter<Push, T> : public Pusher<T> {
 public:
  explicit Filter(std::function<bool(T)> predicate, Pusher<T>* downstream)
      : predicate_(predicate), downstream_(downstream) {}

  void push(T x) override {
    if (predicate_(x)) {
      downstream_->push(x);
    }
  }

 private:
  std::function<bool(T)> predicate_;
  Pusher<T>* downstream_;
};

template <typename T>
Filter<Push, T> make_filter(std::function<bool(T)> predicate,
                            Pusher<T>* downstream) {
  return Filter<Push, T>(predicate, downstream);
}

template <typename T>
class Filter<Pull, T> : public Puller<T> {
 public:
  explicit Filter(std::function<bool(T)> predicate, Puller<T>* upstream)
      : predicate_(predicate), upstream_(upstream) {}

  boost::optional<T> pull() override {
    boost::optional<T> x = upstream_->pull();
    if (x && predicate_(*x)) {
      return x;
    } else {
      return {};
    }
  }

 private:
  std::function<bool(T)> predicate_;
  Puller<T>* upstream_;
};

template <typename T>
Filter<Pull, T> make_filter(std::function<bool(T)> predicate,
                            Puller<T>* upstream) {
  return Filter<Pull, T>(predicate, upstream);
}

}  // namespace latticeflow

#endif  // PUSHPULL_FILTER_H_
