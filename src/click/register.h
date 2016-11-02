#ifndef CLICK_REGISTER_H_
#define CLICK_REGISTER_H_

#include "boost/optional.hpp"

#include "click/phantoms.h"
#include "click/puller.h"
#include "click/pusher.h"

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

#endif  // CLICK_REGISTER_H_
