#ifndef CLICK_FILTER_H_
#define CLICK_FILTER_H_

#include <type_traits>

#include "click/pushable.h"

namespace latticeflow {

// `Map(f, d).push(x)` performs `d->push(f(x))` where `x` is an lvalue or
// rvalue reference and is forwarded to `f`. Note that the return of `f` is not
// owned by the `Map` element, it is moved downstream.
template <typename From, typename To, typename F>
class Map : public Pushable<From&&> {
 public:
  Map(F&& f, Pushable<To>* downstream)
      : f_(std::forward<F>(f)), downstream_(downstream) {}

  void push(From&& x) override { downstream_->push(f_(std::forward<From>(x))); }

 private:
  F f_;
  Pushable<To>* downstream_;
};

template <typename From, typename To, typename F>
Map<From, To, F> make_map(F&& f, Pushable<To>* downstream) {
  return Map<From, To, F>(std::forward<F>(f), downstream);
}

}  // namespace latticeflow

#endif  // CLICK_FILTER_H_
