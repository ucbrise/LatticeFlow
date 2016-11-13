#ifndef CLICK_LAMBDA_H_
#define CLICK_LAMBDA_H_

#include "boost/optional.hpp"

#include "click/phantoms.h"
#include "click/puller.h"
#include "click/pusher.h"

namespace latticeflow {

template <typename T, typename F>
class Lambda : public Pusher<T> {
 public:
  explicit Lambda(F&& f) : f_(std::forward<F>(f)) {}
  void push(T&& x) override { f_(std::forward<T>(x)); }

 private:
  F f_;
};

template <typename T, typename F>
Lambda<T, F> make_lambda(F&& f) {
  return Lambda(std::forward<F>(f));
}

}  // namespace latticeflow

#endif  // CLICK_LAMBDA_H_
