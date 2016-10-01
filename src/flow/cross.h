#ifndef FLOW_CROSS_H_
#define FLOW_CROSS_H_

#include <memory>
#include <utility>

#include <boost/coroutine2/all.hpp>

#include "flow/operator.h"

namespace latticeflow {

template <typename A, typename B>
class Cross : public Operator<std::pair<A, B>> {
 public:
  Cross(Operator<A>* const left, Operator<B>* const right)
      : left_(left),
        right_(right),
        coroutine_([this](push_type& yield) { NextCoroutine(yield); }),
        coroutine_iterator_(begin(coroutine_)) {}
  Cross(const Cross<A, B>&) = delete;
  Cross& operator=(const Cross<A, B>&) = delete;

  boost::optional<std::pair<A, B>> next() override {
    if (coroutine_iterator_ != end(coroutine_)) {
      std::pair<A, B> p = *coroutine_iterator_;
      ++coroutine_iterator_;
      return p;
    } else {
      return {};
    }
  }

  void reset() override {
    left_->reset();
    right_->reset();
    coroutine_ = pull_type([this](push_type& yield) { NextCoroutine(yield); });
    coroutine_iterator_ = begin(coroutine_);
  }

 private:
  using coroutine_type = boost::coroutines2::coroutine<std::pair<A, B>>;
  using push_type = typename coroutine_type::push_type;
  using pull_type = typename coroutine_type::pull_type;
  using iterator = typename pull_type::iterator;

  void NextCoroutine(typename coroutine_type::push_type& yield) {
    boost::optional<A> left_val = left_->next();
    while (left_val) {
      right_->reset();
      boost::optional<B> right_val = right_->next();
      while (right_val) {
        yield(std::make_pair(*left_val, *right_val));
        right_val = right_->next();
      }
      left_val = left_->next();
    }
  }

  Operator<A>* const left_;
  Operator<B>* const right_;
  pull_type coroutine_;
  iterator coroutine_iterator_;
};

}  // namespace latticeflow

#endif  // FLOW_CROSS_H_
