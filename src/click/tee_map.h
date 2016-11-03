#ifndef CLICK_ROUND_TEE_MAP_H_
#define CLICK_ROUND_TEE_MAP_H_

#include <utility>
#include <vector>

#include "boost/optional.hpp"

#include "click/phantoms.h"
#include "click/puller.h"
#include "click/pusher.h"

namespace latticeflow {

template <typename From, typename Left, typename Right, typename F>
class TeeMap : public Pusher<From> {
 public:
  explicit TeeMap(F&& f, Pusher<Left>* left, Pusher<Right>* right, )
      : f_(f), left_(left), right_(right) {}

  void push(From&& x) override {
    std::pair<Left, Right> p = f_(std::forward<From>(x));
    left_.push(std::get<0>(p));
    right_.push(std::get<1>(p));
  }

 private:
  F&& f_;
  Pusher<Left>* left_;
  Pusher<Right>* right_;
};

template <typename T>
Tee<Push, T> make_tee(std::vector<Pusher<T>*> downstreams) {
  return Tee<Push, T>(std::move(downstreams));
}

}  // namespace latticeflow

#endif  // CLICK_ROUND_TEE_MAP_H_
