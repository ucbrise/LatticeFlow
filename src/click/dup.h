#ifndef CLICK_DUP_H_
#define CLICK_DUP_H_

#include "click/pushable.h"

namespace latticeflow {

template <typename From, typename To>
class Dup : public Pushable<From&&> {
  // TODO(mwhittaker): Figure out if we should include this. See comment in
  // tee.h.
  static_assert(!std::is_rvalue_reference<From&&>::value ||
                    std::is_reference<To>::value,
                "You cannot create a `Dup` which forwards rvalue references to "
                "downstream `Pushable`s that take arguments by value. In most "
                "circumstances, this is a bug. You should not be "
                "move-constructing multiple objects from the same "
                "rvalue-reference.");

 public:
  explicit Dup(Pushable<To>* downstream, const int num_dups)
      : downstream_(downstream), num_dups_(num_dups) {}

  void push(From&& x) override {
    for (int i = 0; i < num_dups_; ++i) {
      downstream_->push(std::forward<From>(x));
    }
  }

 private:
  Pushable<To>* downstream_;
  const int num_dups_;
};

template <typename From, typename To>
Dup<From, To> make_dup(Pushable<To>* downstream, const int num_dups) {
  return Dup<From, To>(downstream, num_dups);
}

}  // namespace latticeflow

#endif  // CLICK_DUP_H_
