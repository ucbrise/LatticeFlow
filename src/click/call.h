#ifndef CLICK_CALL_H_
#define CLICK_CALL_H_

#include <type_traits>

#include "click/pushable.h"

namespace latticeflow {

// `Call(f).push(x)` performs `f(x)` where `x` is an lvalue or rvalue reference
// and is forwarded to `f`.
template <typename T, typename F>
class Call : public Pushable<T&&> {
 public:
  explicit Call(F&& f) : f_(std::forward<F>(f)) {}
  void push(T&& x) override { f_(std::forward<T>(x)); }

 private:
  F f_;
};

}  // namespace latticeflow

#endif  // CLICK_CALL_H_
