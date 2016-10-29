#ifndef PUSHPULL_DUP_H_
#define PUSHPULL_DUP_H_

#include "boost/optional.hpp"

#include "pushpull/phantoms.h"
#include "pushpull/puller.h"
#include "pushpull/pusher.h"

namespace latticeflow {

template <typename Direction, typename T>
class Dup;

template <typename T>
class Dup<Push, T> : public Pusher<T> {
 public:
  explicit Dup(Pusher<T>* downstream, const int num_dups)
      : downstream_(downstream), num_dups_(num_dups) {}

  void push(T x) override {
    for (int i = 0; i < num_dups_; ++i) {
      downstream_->push(x);
    }
  }

 private:
  Pusher<T>* downstream_;
  const int num_dups_;
};

template <typename T>
Dup<Push, T> make_dup(Pusher<T>* downstream, const int num_dups) {
  return Dup<Push, T>(downstream, num_dups);
}

template <typename T>
class Dup<Pull, T> : public Puller<T> {
 public:
  explicit Dup(Puller<T>* upstream, const int num_dups)
      : upstream_(upstream), num_dups_(num_dups) {}

  boost::optional<T> pull() override {
    if (num_dups_sent_ == num_dups_) {
      num_dups_sent_ = 0;
    }
    if (num_dups_sent_ == 0) {
      buf_ = upstream_->pull();
    }
    num_dups_sent_++;
    return buf_;
  }

 private:
  Puller<T>* upstream_;
  const int num_dups_;

  int num_dups_sent_ = 0;
  boost::optional<T> buf_;
};

template <typename T>
Dup<Pull, T> make_dup(Puller<T>* upstream, const int num_dups) {
  return Dup<Pull, T>(upstream, num_dups);
}

}  // namespace latticeflow

#endif  // PUSHPULL_DUP_H_
