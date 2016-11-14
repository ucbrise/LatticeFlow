#ifndef CLICK_DEDUP_H_
#define CLICK_DEDUP_H_

#include <cassert>
#include <map>

#include "click/pushable.h"

namespace latticeflow {

template <typename From, typename To>
class Dedup : public Pushable<From> {
  // TODO(mwhittaker): I don't think Dedup shouldn be owning everything passed
  // to it. Think about how to make this better.
  static_assert(
      !std::is_reference<From>::value,
      "Dedup takes ownership of values, so they must be passed by value.");

 public:
  explicit Dedup(Pushable<To>* downstream, const int num_dups)
      : downstream_(downstream), num_dups_(num_dups) {
    // TODO(mwhittaker): User proper assertions.
    assert(num_dups > 1);
  }

  void push(From x) override {
    auto it = counts_.find(x);
    if (it == std::end(counts_)) {
      counts_.insert(std::make_pair(std::move(x), 1));
    } else {
      it->second++;
      if (it->second == num_dups_) {
        downstream_->push(std::move(x));
        counts_.erase(it);
      }
    }
  }

 private:
  Pushable<To>* downstream_;
  const int num_dups_;
  std::map<From, int> counts_;
};

template <typename From, typename To>
Dedup<From, To> make_dedup(Pushable<To>* downstream, const int num_dups) {
  return Dedup<From, To>(downstream, num_dups);
}

}  // namespace latticeflow

#endif  // CLICK_DEDUP_H_
