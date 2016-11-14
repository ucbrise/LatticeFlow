#ifndef PUSHPULL_REGISTER_H_
#define PUSHPULL_REGISTER_H_

#include "boost/optional.hpp"

#include "pushpull/phantoms.h"
#include "pushpull/puller.h"
#include "pushpull/pusher.h"

namespace latticeflow {

template <typename Direction, typename T>
class Register;

template <typename T>
class Register<Push, T> : public Pusher<T> {
 public:
  void push(T x) override { x_ = x; }
  const T& get() { return x_; }

 private:
  T x_;
};

template <typename T>
class Register<Pull, T> : public Puller<T> {
 public:
  explicit Register(T x) : x_(x) {}

  boost::optional<T> pull() override { return x_; }

 private:
  T x_;
};

}  // namespace latticeflow

#endif  // PUSHPULL_REGISTER_H_
