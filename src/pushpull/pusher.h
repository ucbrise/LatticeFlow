#ifndef PUSHPULL_PUSHER_H_
#define PUSHPULL_PUSHER_H_

namespace latticeflow {

template <typename T>
class Pusher {
 public:
  virtual void push(T x) = 0;
};

}  // namespace latticeflow

#endif  // PUSHPULL_PUSHER_H_
