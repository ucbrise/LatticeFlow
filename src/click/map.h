#ifndef CLICK_FILTER_H_
#define CLICK_FILTER_H_

#include <functional>
#include <type_traits>

#include "boost/optional.hpp"

#include "click/phantoms.h"
#include "click/puller.h"
#include "click/pusher.h"

namespace latticeflow {

template <typename From, typename To, typename F>
class Map : public Pusher<From> {
 public:
  Map(F&& f, Pusher<To>* downstream)
      : f_(std::forward<F>(f)), downstream_(downstream) {}

  void push(From&& x) override {
    typename std::result_of<F(From &&)>::type y = f_(std::forward<From>(x));
    downstream_->push(std::forward<To>(y));
  }

 private:
  F f_;
  Pusher<To&&>* downstream_;
};

template <typename From, typename To, typename F>
Map<From, To, F> make_map(F&& f, Pusher<To>* downstream) {
  return {std::forward<F>(f), downstream};
}

}  // namespace latticeflow

#endif  // CLICK_FILTER_H_
