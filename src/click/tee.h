#ifndef CLICK_ROUND_TEE_H_
#define CLICK_ROUND_TEE_H_

#include <type_traits>
#include <utility>
#include <vector>

#include "click/pushable.h"

namespace latticeflow {

// `Tee`, inspired by the command line utility `tee`, copies its input to a
// number of downstream `Pushable`s. For example, `Tee({a, b}).push(x)`
// performs `a.push(x); b.push(x)`. Note that `x` is a universal reference that
// is perfect forwarded to all downstream `Pushable`s. Moreover, downstream
// `Pushable`s are pushed in the order they are provided to the constructor.
//
// Note that a Tee constructed from an empty list of downstreams acts like a
// `Drop`. And a `Tee` constructed from a singleton list acts like `Id`.
template <typename From, typename To>
class Tee : public Pushable<From&&> {
  // TODO(mwhittaker): Figure out if we should include this. If `From` is
  // instantiated with something like `unique_ptr`, then this `static_assert`
  // is almost certainly catching a bug. However, if `From` is something like
  // `int`, then it can be moved from multiple times without any problem.
  static_assert(!std::is_rvalue_reference<From&&>::value ||
                    std::is_reference<To>::value,
                "You cannot create a `Tee` which forwards rvalue references to "
                "downstream `Pushable`s that take arguments by value. In most "
                "circumstances, this is a bug. You should not be "
                "move-constructing multiple objects from the same "
                "rvalue-reference.");

 public:
  explicit Tee(std::vector<Pushable<To>*> downstreams)
      : downstreams_(std::move(downstreams)) {}

  void push(From&& x) override {
    for (Pushable<To>* downstream : downstreams_) {
      downstream->push(std::forward<To>(x));
    }
  }

 private:
  std::vector<Pushable<To>*> downstreams_;
};

}  // namespace latticeflow

#endif  // CLICK_ROUND_TEE_H_
