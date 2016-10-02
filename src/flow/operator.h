#ifndef FLOW_OPERATOR_H_
#define FLOW_OPERATOR_H_

#include <tuple>

#include "boost/optional.hpp"

namespace latticeflow {

template <typename... Ts>
class Operator {
 public:
  virtual boost::optional<std::tuple<Ts...>> next() = 0;
  virtual void reset() = 0;
};

}  // namespace latticeflow

#endif  // FLOW_OPERATOR_H_
