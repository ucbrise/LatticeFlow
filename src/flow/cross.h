#ifndef FLOW_CROSS_H_
#define FLOW_CROSS_H_

#include <utility>

#include "flow/operator.h"

namespace latticeflow {

template <typename A, typename B>
class Cross : public Operator<std::pair<A, B>> {
 public:
  Cross(Operator<A>* const left, Operator<B>* const right)
      : left_(left), right_(right) {}
  Cross(const Cross<A, B>&) = delete;
  Cross& operator=(const Cross<A, B>&) = delete;

  boost::optional<std::pair<A, B>> next() override {
    // Initially, `left_->next()` has not been advanced, so `left_val_` will be
    // empty when this method is called for the very first time.
    if (!left_val_) {
      left_val_ = left_->next();

      // If `left_` is empty, `left_->next()` will never return anything.
      if (!left_val_) {
        return {};
      }
    }

    boost::optional<B> right_val = right_->next();
    if (!right_val) {
      // If we've exhausted `right_`, then we need to advance `left_` and reset
      // `right_`. If `left_` is also exhausted, then we're done.
      left_val_ = left_->next();
      if (!left_val_) {
        return {};
      }

      right_->reset();
      right_val = right_->next();
      // If `right_` is empty, calling `next` after `reset` won't return
      // anything.
      if (!right_val) {
        return {};
      }
    }

    return std::make_pair(*left_val_, *right_val);
  }

  void reset() override {
    left_->reset();
    right_->reset();
  }

 private:
  Operator<A>* const left_;
  Operator<B>* const right_;
  boost::optional<A> left_val_;
};

}  // namespace latticeflow

#endif  // FLOW_CROSS_H_
