#ifndef FLOW_COROUTINE_OPERATOR_H_
#define FLOW_COROUTINE_OPERATOR_H_

#include <memory>
#include <tuple>

#include "boost/coroutine2/all.hpp"
#include "boost/optional.hpp"

#include "flow/operator.h"

namespace latticeflow {

// TODO(mwhittaker): Document.
template <typename... Ts>
class CoroutineOperator : public Operator<Ts...> {
 public:
  // TODO(mwhittaker): Document why we can't construct the coroutine_ with the
  // correct lambda yet.
  CoroutineOperator()
      : coroutine_([this](push_type&) {}),
        coroutine_iterator_(begin(coroutine_)) {}

  boost::optional<std::tuple<Ts...>> next() override {
    if (coroutine_iterator_ != end(coroutine_)) {
      std::tuple<Ts...> p = *coroutine_iterator_;
      ++coroutine_iterator_;
      return p;
    } else {
      return {};
    }
  }

  virtual void reset() override {
    coroutine_ = pull_type([this](push_type& yield) { YieldNext(yield); });
    coroutine_iterator_ = begin(coroutine_);
  }

 protected:
  using coroutine_type = boost::coroutines2::coroutine<std::tuple<Ts...>>;
  using push_type = typename coroutine_type::push_type;
  using pull_type = typename coroutine_type::pull_type;
  using iterator = typename pull_type::iterator;

 private:
  virtual void YieldNext(push_type& yield) = 0;

  pull_type coroutine_;
  iterator coroutine_iterator_;
};

};  // namespace latticeflow

#endif  // FLOW_COROUTINE_OPERATOR_H_
