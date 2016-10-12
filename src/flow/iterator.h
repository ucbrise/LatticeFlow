#ifndef FLOW_ITERATOR_H_
#define FLOW_ITERATOR_H_

#include "flow/operator.h"

namespace latticeflow {

template <typename ForwardIterator, typename... Ts>
class Iterator : public Operator<Ts...> {
 public:
  explicit Iterator(const ForwardIterator& begin, const ForwardIterator& end)
      : begin_(begin), end_(end), it_(begin) {}
  Iterator(const Iterator&) = delete;
  Iterator& operator=(const Iterator&) = delete;

  boost::optional<std::tuple<Ts...>> next() override {
    if (it_ != end_) {
      boost::optional<std::tuple<Ts...>> o(*it_);
      it_++;
      return o;
    } else {
      return {};
    }
  }

  void reset() override { it_ = begin_; }

 private:
  const ForwardIterator begin_;
  const ForwardIterator end_;
  ForwardIterator it_;
};

}  // namespace latticeflow

#endif  // FLOW_ITERATOR_H_
