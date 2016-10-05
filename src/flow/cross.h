#ifndef FLOW_CROSS_H_
#define FLOW_CROSS_H_

#include <memory>
#include <utility>

#include <boost/coroutine2/all.hpp>

#include "flow/coroutine_operator.h"
#include "flow/operator.h"
#include "flow/template_helpers.h"

namespace latticeflow {

// http://stackoverflow.com/a/21023429/3187068
// TODO(mwhittaker): Understand this, then document it.
template <typename Left, typename Right>
class Cross;

template <typename... Lefts, typename... Rights>
class Cross<left<Lefts...>, right<Rights...>>
    : public CoroutineOperator<Lefts..., Rights...> {
 public:
  Cross(Operator<Lefts...>* const left, Operator<Rights...>* const right)
      : left_(left), right_(right) {
    reset();
  }
  Cross(const Cross<left<Lefts...>, right<Rights...>>&) = delete;
  Cross& operator=(const Cross<left<Lefts...>, right<Rights...>>&) = delete;

  void reset() override {
    left_->reset();
    right_->reset();
    CoroutineOperator<Lefts..., Rights...>::reset();
  }

 private:
  using push_type = typename CoroutineOperator<Lefts..., Rights...>::push_type;

  void YieldNext(push_type& yield) override {
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
};

}  // namespace latticeflow

#endif  // FLOW_CROSS_H_
