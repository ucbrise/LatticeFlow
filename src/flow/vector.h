#ifndef FLOW_VECTOR_H_
#define FLOW_VECTOR_H_

#include <vector>

#include "flow/operator.h"

namespace latticeflow {

template <typename... Ts>
class Vector : public Operator<Ts...> {
 public:
  explicit Vector(const std::vector<std::tuple<Ts...>>& xs)
      : xs_(xs), it_(xs_.begin()) {}
  Vector(const Vector<Ts...>&) = delete;
  Vector& operator=(const Vector<Ts...>&) = delete;

  boost::optional<std::tuple<Ts...>> next() override {
    if (it_ != xs_.end()) {
      boost::optional<std::tuple<Ts...>> o(*it_);
      it_++;
      return o;
    } else {
      return {};
    }
  }

  void reset() override { it_ = xs_.begin(); }

 private:
  std::vector<std::tuple<Ts...>> xs_;
  typename std::vector<std::tuple<Ts...>>::iterator it_;
};

}  // namespace latticeflow

#endif  // FLOW_VECTOR_H_
