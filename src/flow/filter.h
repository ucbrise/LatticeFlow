#ifndef FLOW_FILTER_H_
#define FLOW_FILTER_H_

#include <functional>

#include "flow/operator.h"

namespace latticeflow {

template <typename... Ts>
class Filter : public Operator<Ts...> {
 public:
  Filter(Operator<Ts...>* const child,
         std::function<bool(const std::tuple<Ts...>&)> filter)
      : child_(child), filter_(filter) {}
  Filter(const Filter<Ts...>&) = delete;
  Filter& operator=(const Filter<Ts...>&) = delete;

  boost::optional<std::tuple<Ts...>> next() override {
    boost::optional<std::tuple<Ts...>> x = child_->next();
    while (x) {
      if (filter_(*x)) {
        return x;
      }
      x = child_->next();
    }
    return {};
  }

  void reset() override { child_->reset(); }

 private:
  Operator<Ts...>* const child_;
  std::function<bool(const std::tuple<Ts...>&)> filter_;
};

}  // namespace latticeflow

#endif  // FLOW_FILTER_H_
