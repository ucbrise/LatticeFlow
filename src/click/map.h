#ifndef CLICK_FILTER_H_
#define CLICK_FILTER_H_

#include <functional>

#include "boost/optional.hpp"

#include "click/phantoms.h"
#include "click/puller.h"
#include "click/pusher.h"

namespace latticeflow {

template <typename Direction, typename From, typename To>
class Map;

template <typename From, typename To>
class Map<Push, From, To> : public Pusher<From> {
 public:
  explicit Map(std::function<To(From)> f, Pusher<To>* downstream)
      : f_(f), downstream_(downstream) {}

  void push(From x) override { downstream_->push(f_(x)); }

 private:
  std::function<To(From)> f_;
  Pusher<To>* downstream_;
};

template <typename From, typename To>
Map<Push, From, To> make_map(std::function<To(From)> f,
                             Pusher<To>* downstream) {
  return Map<Push, From, To>(f, downstream);
}

template <typename From, typename To>
class Map<Pull, From, To> : public Puller<To> {
 public:
  explicit Map(std::function<To(From)> f, Puller<From>* upstream)
      : f_(f), upstream_(upstream) {}

  boost::optional<To> pull() override {
    boost::optional<From> x = upstream_->pull();
    if (x) {
      return f_(*x);
    } else {
      return {};
    }
  }

 private:
  std::function<To(From)> f_;
  Puller<From>* upstream_;
};

template <typename From, typename To>
Map<Pull, From, To> make_map(std::function<To(From)> f,
                             Puller<From>* upstream) {
  return Map<Pull, From, To>(f, upstream);
}

}  // namespace latticeflow

#endif  // CLICK_FILTER_H_
