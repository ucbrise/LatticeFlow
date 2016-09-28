#ifndef MAP_VECTOR_H_
#define MAP_VECTOR_H_

#include <functional>

#include "flow/operator.h"

namespace latticeflow {

template <typename A, typename B>
class Map : public Operator<B> {
 public:
  Map(Operator<A>* const child, std::function<B(const A&)> f)
      : child_(child), f_(f) {}
  Map(const Map<A, B>& v) = delete;
  Map& operator=(const Map<A, B>& v) = delete;

  boost::optional<B> next() override {
    boost::optional<A> x = child_->next();
    if (x) {
      return f_(*x);
    } else {
      return {};
    }
  }

  void reset() override { child_->reset(); }

 private:
  Operator<A>* const child_;
  std::function<B(const A&)> f_;
};

}  // namespace latticeflow

#endif  // MAP_VECTOR_H_
