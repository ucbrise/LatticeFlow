#ifndef PUSHPULL_IF_H_
#define PUSHPULL_IF_H_

#include <functional>

#include "boost/optional.hpp"

#include "pushpull/phantoms.h"
#include "pushpull/puller.h"
#include "pushpull/pusher.h"

namespace latticeflow {

template <typename Flow, typename T>
class If;

template <typename T>
class If<Push, T> : public Pusher<T> {
 public:
  If(std::function<bool(T)> predicate, Pusher<T>* true_branch,
     Pusher<T>* false_branch)
      : predicate_(predicate),
        true_branch_(true_branch),
        false_branch_(false_branch) {}

  void push(T x) override {
    if (predicate_(x)) {
      true_branch_->push(x);
    } else {
      false_branch_->push(x);
    }
  }

 private:
  std::function<bool(T)> predicate_;
  Pusher<T>* true_branch_;
  Pusher<T>* false_branch_;
};

template <typename T>
If<Push, T> make_if(std::function<bool(T)> predicate, Pusher<T>* true_branch,
                    Pusher<T>* false_branch) {
  return If<Push, T>(predicate, true_branch, false_branch);
}

}  // namespace latticeflow

#endif  // PUSHPULL_IF_H_
