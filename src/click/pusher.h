#ifndef CLICK_PUSHER_H_
#define CLICK_PUSHER_H_

#include <type_traits>

namespace latticeflow {

template <typename T>
class Pusher {
  static_assert(std::is_reference<T>::value, "Type T is not a reference.");

 public:
  virtual void push(T&& x) = 0;
};

}  // namespace latticeflow

#endif  // CLICK_PUSHER_H_
