#ifndef FLOW_EMPTY_H_
#define FLOW_EMPTY_H_

#include "flow/operator.h"

namespace latticeflow {

template <typename... Ts>
class Empty : public Operator<Ts...> {
 public:
  Empty() = default;
  Empty(const Empty<Ts...>&) = delete;
  Empty& operator=(const Empty<Ts...>&) = delete;

  boost::optional<std::tuple<Ts...>> next() override { return {}; }

  void reset() override {}
};

}  // namespace latticeflow

#endif  // FLOW_EMPTY_H_
