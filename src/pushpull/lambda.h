#ifndef PUSHPULL_LAMBDA_H_
#define PUSHPULL_LAMBDA_H_

#include "boost/optional.hpp"

#include "pushpull/phantoms.h"
#include "pushpull/puller.h"
#include "pushpull/pusher.h"

namespace latticeflow {

template <typename Direction, typename T>
class Lambda;

template <typename T>
class Lambda<Push, T> : public Pusher<T> {
 public:
  explicit Lambda(std::function<void(T)> f) : f_(f) {}
  void push(T x) override { f_(x); }

 private:
  std::function<void(T)> f_;
};

template <typename T>
class Lambda<Pull, T> : public Puller<T> {
 public:
  explicit Lambda(std::function<boost::optional<T>()> f) : f_(f) {}

  boost::optional<T> pull() override { return f_(); }

 private:
  std::function<boost::optional<T>()> f_;
};

}  // namespace latticeflow

#endif  // PUSHPULL_LAMBDA_H_
