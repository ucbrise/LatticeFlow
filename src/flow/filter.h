#ifndef FILTER_VECTOR_H_
#define FILTER_VECTOR_H_

#include <functional>

#include "flow/operator.h"

namespace latticeflow {

template <typename T>
class Filter : public Operator<T> {
 public:
  Filter(Operator<T>* const child, std::function<bool(const T&)> filter)
      : child_(child), filter_(filter) {}
  Filter(const Filter<T>& v) = delete;
  Filter& operator=(const Filter<T>& v) = delete;

  boost::optional<T> next() override {
    boost::optional<T> x = child_->next();
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
  Operator<T>* const child_;
  std::function<bool(const T&)> filter_;
};

}  // namespace latticeflow

#endif  // FILTER_VECTOR_H_
