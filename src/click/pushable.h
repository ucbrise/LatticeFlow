#ifndef CLICK_PUSHABLE_H_
#define CLICK_PUSHABLE_H_

namespace latticeflow {

// Pushables are the duals of iterators (which could be called Pullables). We
// repeatedly *pull* values from iterators. We repeatedly *push* values to
// Pushables.
//
// Note that Pushables are parametrized on a template parameter `T` which can
// be instantiated with a lot of different types:
//   - int
//   - int&
//   - int&&
//   - const int
//   - const int&
//   - const int&&
//   - int*
//   - int**
template <typename T>
class Pushable {
 public:
  virtual void push(T x) = 0;
};

}  // namespace latticeflow

#endif  // CLICK_PUSHABLE_H_
