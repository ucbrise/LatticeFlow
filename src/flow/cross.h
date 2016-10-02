#ifndef FLOW_CROSS_H_
#define FLOW_CROSS_H_

#include <memory>
#include <utility>

#include <boost/coroutine2/all.hpp>

#include "flow/operator.h"
#include "flow/template_helpers.h"

namespace latticeflow {

// http://stackoverflow.com/a/21023429/3187068
// TODO(mwhittaker): Understand this, then document it.
template <typename Left, typename Right>
class Cross {};

template <typename... Lefts, typename... Rights>
class Cross<left<Lefts...>, right<Rights...>>
    : public Operator<Lefts..., Rights...> {
 public:
  Cross(Operator<Lefts...>* const left, Operator<Rights...>* const right)
      : left_(left),
        right_(right),
        coroutine_([this](push_type& yield) { NextCoroutine(yield); }),
        coroutine_iterator_(begin(coroutine_)) {}
  Cross(const Cross<left<Lefts...>, right<Rights...>>&) = delete;
  Cross& operator=(const Cross<left<Lefts...>, right<Rights...>>&) = delete;

  boost::optional<std::tuple<Lefts..., Rights...>> next() override {
    if (coroutine_iterator_ != end(coroutine_)) {
      std::tuple<Lefts..., Rights...> p = *coroutine_iterator_;
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
  using coroutine_type =
      boost::coroutines2::coroutine<std::tuple<Lefts..., Rights...>>;
  using push_type = typename coroutine_type::push_type;
  using pull_type = typename coroutine_type::pull_type;
  using iterator = typename pull_type::iterator;

  void NextCoroutine(typename coroutine_type::push_type& yield) {
    boost::optional<std::tuple<Lefts...>> left_val = left_->next();
    while (left_val) {
      right_->reset();
      boost::optional<std::tuple<Rights...>> right_val = right_->next();
      while (right_val) {
        yield(std::tuple_cat(*left_val, *right_val));
        right_val = right_->next();
      }
      left_val = left_->next();
    }
  }

  Operator<Lefts...>* const left_;
  Operator<Rights...>* const right_;
  pull_type coroutine_;
  iterator coroutine_iterator_;
};

}  // namespace latticeflow

#endif  // FLOW_CROSS_H_
