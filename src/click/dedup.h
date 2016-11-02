#ifndef CLICK_DEDUP_H_
#define CLICK_DEDUP_H_

#include <map>

#include "boost/optional.hpp"

#include "click/phantoms.h"
#include "click/puller.h"
#include "click/pusher.h"

namespace latticeflow {

template <typename Direction, typename T>
class Dedup;

template <typename T>
class Dedup<Push, T> : public Pusher<T> {
 public:
  explicit Dedup(Pusher<T>* downstream, const int num_dups)
      : downstream_(downstream), num_dups_(num_dups) {}

  void push(T x) override {
    counts_[x]++;
    if (counts_[x] == num_dups_) {
      counts_[x] = 0;
      downstream_->push(x);
    }
  }

 private:
  Pusher<T>* downstream_;
  const int num_dups_;
  std::map<T, int> counts_;
};

template <typename T>
Dedup<Push, T> make_dedup(Pusher<T>* downstream, const int num_dups) {
  return Dedup<Push, T>(downstream, num_dups);
}

template <typename T>
class Dedup<Pull, T> : public Puller<T> {
 public:
  explicit Dedup(Puller<T>* upstream, const int num_dups)
      : upstream_(upstream), num_dups_(num_dups) {}

  boost::optional<T> pull() override {
    boost::optional<T> x = upstream_->pull();
    counts_[x]++;
    if (counts_[x] == num_dups_) {
      counts_[x] = 0;
      return x;
    } else {
      return {};
    }
  }

 private:
  Puller<T>* upstream_;
  const int num_dups_;
  std::map<boost::optional<T>, int> counts_;
};

template <typename T>
Dedup<Pull, T> make_dedup(Puller<T>* upstream, const int num_dups) {
  return Dedup<Pull, T>(upstream, num_dups);
}

}  // namespace latticeflow

#endif  // CLICK_DEDUP_H_
