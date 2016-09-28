#ifndef FLOW_OPERATOR_H_
#define FLOW_OPERATOR_H_

#include "boost/optional.hpp"

namespace latticeflow {

template <typename T>
class Operator {
 public:
  virtual boost::optional<T> next() = 0;
  virtual void reset() = 0;
};

}  // namespace latticeflow

#endif  // FLOW_OPERATOR_H_
