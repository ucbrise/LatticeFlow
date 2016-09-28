#ifndef FLOW_EMPTY_H_
#define FLOW_EMPTY_H_

#include "flow/operator.h"

namespace latticeflow {

template <typename T>
class Empty : public Operator<T> {
 public:
  Empty() = default;
  Empty(const Empty<T>& v) = delete;
  Empty& operator=(const Empty<T>& v) = delete;

  boost::optional<T> next() override { return {}; }

  void reset() override {}
};

}  // namespace latticeflow

#endif  // FLOW_EMPTY_H_
