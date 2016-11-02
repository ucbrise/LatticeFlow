#ifndef CLICK_ROUND_TEE_H_
#define CLICK_ROUND_TEE_H_

#include <vector>

#include "boost/optional.hpp"

#include "click/phantoms.h"
#include "click/puller.h"
#include "click/pusher.h"

namespace latticeflow {

template <typename Direction, typename T>
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

#endif  // CLICK_ROUND_TEE_H_
