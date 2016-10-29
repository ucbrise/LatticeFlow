#ifndef PUSHPULL_ROUND_ROBIN_H_
#define PUSHPULL_ROUND_ROBIN_H_

#include <vector>

#include "boost/optional.hpp"

#include "pushpull/phantoms.h"
#include "pushpull/puller.h"
#include "pushpull/pusher.h"

namespace latticeflow {

template <typename Direction, typename T>
class RoundRobin;

template <typename T>
class RoundRobin<Push, T> : public Pusher<T> {
 public:
  explicit RoundRobin(std::vector<Pusher<T>*> downstreams)
      : downstreams_(std::move(downstreams)) {}

  void push(T x) override {
    if (downstreams_.size() > 0) {
      downstreams_[index_]->push(x);
      index_ = (index_ + 1) % downstreams_.size();
    }
  }

 private:
  std::vector<Pusher<T>*> downstreams_;
  int index_ = 0;
};

template <typename T>
RoundRobin<Push, T> make_round_robin(std::vector<Pusher<T>*> downstreams) {
  return RoundRobin<Push, T>(std::move(downstreams));
}

template <typename T>
class RoundRobin<Pull, T> : public Puller<T> {
 public:
  explicit RoundRobin(std::vector<Puller<T>*> upstreams)
      : upstreams_(std::move(upstreams)) {}

  boost::optional<T> pull() override {
    if (upstreams_.size() > 0) {
      boost::optional<T> x = upstreams_[index_]->pull();
      index_ = (index_ + 1) % upstreams_.size();
      return x;
    } else {
      return {};
    }
  }

 private:
  std::vector<Puller<T>*> upstreams_;
  int index_ = 0;
};

template <typename T>
RoundRobin<Pull, T> make_round_robin(std::vector<Puller<T>*> upstreams) {
  return RoundRobin<Pull, T>(std::move(upstreams));
}

}  // namespace latticeflow

#endif  // PUSHPULL_ROUND_ROBIN_H_
