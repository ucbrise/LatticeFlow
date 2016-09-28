#ifndef FLOW_VECTOR_H_
#define FLOW_VECTOR_H_

#include <vector>

#include "flow/operator.h"

namespace latticeflow {

template <typename T>
class Vector : public Operator<T> {
 public:
  explicit Vector(const std::vector<T>& xs) : xs_(xs), it_(xs_.begin()) {}
  Vector(const Vector<T>& v) = delete;
  Vector& operator=(const Vector<T>& v) = delete;

  boost::optional<T> next() override {
    if (it_ != xs_.end()) {
      boost::optional<T> o(*it_);
      it_++;
      return o;
    } else {
      return {};
    }
  }

 private:
  std::vector<T> xs_;
  typename std::vector<T>::iterator it_;
};

}  // namespace latticeflow

#endif  // FLOW_VECTOR_H_
