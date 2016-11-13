#ifndef CLICK_DROP_H_
#define CLICK_DROP_H_

#include <type_traits>

#include "click/pushable.h"

namespace latticeflow {

// `Drop().push(x)` does nothing; it is a no-op.
template <typename T>
class Drop : public Pushable<T&&> {
 public:
  void push(T&&) override {}
};

}  // namespace latticeflow

#endif  // CLICK_DROP_H_
