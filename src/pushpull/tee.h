#ifndef PUSHPULL_ROUND_TEE_H_
#define PUSHPULL_ROUND_TEE_H_

#include <vector>

#include "boost/optional.hpp"

#include "pushpull/phantoms.h"
#include "pushpull/puller.h"
#include "pushpull/pusher.h"

namespace latticeflow {

template <typename Flow, typename T>
class Tee;

template <typename T>
class Tee<Push, T> : public Pusher<T> {
 public:
  explicit Tee(std::vector<Pusher<T>*> downstreams)
      : downstreams_(std::move(downstreams)) {}

  void push(T x) override {
    for (Pusher<T>* downstream : downstreams_) {
      downstream->push(x);
    }
  }

 private:
  std::vector<Pusher<T>*> downstreams_;
};

template <typename T>
Tee<Push, T> make_tee(std::vector<Pusher<T>*> downstreams) {
  return Tee<Push, T>(std::move(downstreams));
}

}  // namespace latticeflow

#endif  // PUSHPULL_ROUND_TEE_H_
