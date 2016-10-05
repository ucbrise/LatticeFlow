#ifndef FLOW_MAP_H_
#define FLOW_MAP_H_

#include <functional>

#include "flow/operator.h"
#include "flow/template_helpers.h"

namespace latticeflow {

template <typename From, typename To>
class Map {};

template <typename... Froms, typename... Tos>
class Map<from<Froms...>, to<Tos...>> : public Operator<Tos...> {
 public:
  Map(Operator<Froms...>* const child,
      std::function<std::tuple<Tos...>(const std::tuple<Froms...>&)> f)
      : child_(child), f_(f) {}
  Map(const Map<from<Froms...>, to<Tos...>>&) = delete;
  Map& operator=(const Map<from<Froms...>, to<Tos...>>&) = delete;

  boost::optional<std::tuple<Tos...>> next() override {
    boost::optional<std::tuple<Froms...>> x = child_->next();
    if (x) {
      return f_(*x);
    } else {
      return {};
    }
  }

  void reset() override { child_->reset(); }

 private:
  Operator<Froms...>* const child_;
  std::function<std::tuple<Tos...>(const std::tuple<Froms...>&)> f_;
};

}  // namespace latticeflow

#endif  // FLOW_MAP_H_
