#ifndef CLICK_ID_H_
#define CLICK_ID_H_

#include <utility>

#include "click/pushable.h"

namespace latticeflow {

// `Id(a).push(x)` performs `a.push(x)` where x is reference that is perfect
// forwarded to `a`.
template <typename From, typename To>
class Id : public Pushable<From> {
 public:
  explicit Id(Pusher<To>* downstream) : downstream_(downstream) {}

  void push(From&& x) override { downstream_->push(std::forward<From>(x)); }

 private:
  Pusher<To>* downstream_;
};

}  // namespace latticeflow

#endif  // CLICK_ID_H_
