#ifndef CLICK_DROP_H_
#define CLICK_DROP_H_

#include <type_traits>

#include <iostream>

#include "click/pushable.h"

namespace latticeflow {

// `Drop().push(x)` does nothing; it is a no-op.
template <typename T>
class Drop : public Pushable<T&&> {
 public:
  void push(T&&) override { std::cout << "dropped" << std::endl; }
};

}  // namespace latticeflow

#endif  // CLICK_DROP_H_
