#ifndef CLICK_DUP_H_
#define CLICK_DUP_H_

#include "boost/optional.hpp"

#include "click/phantoms.h"
#include "click/puller.h"
#include "click/pusher.h"

namespace latticeflow {

template <typename T>
class Dup : public Pusher<T> {
 public:
  explicit Dup(Pusher<T>* downstream, const int num_dups)
      : downstream_(downstream), num_dups_(num_dups) {}

  void push(T&& x) override {
    for (int i = 0; i < num_dups_; ++i) {
      downstream_->push(std::forward<T>(x));
    }
  }

 private:
  Pusher<T>* downstream_;
  const int num_dups_;
};

template <typename T>
Dup<T> make_dup(Pusher<T>* downstream, const int num_dups) {
  return Dup<T>(downstream, num_dups);
}

}  // namespace latticeflow

#endif  // CLICK_DUP_H_
