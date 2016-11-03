#ifndef CLICK_DROP_H_
#define CLICK_DROP_H_

#include "boost/optional.hpp"

#include "click/phantoms.h"
#include "click/puller.h"
#include "click/pusher.h"

namespace latticeflow {

template <typename T>
class Drop : public Pusher<T> {
 public:
  void push(T&&) override {}
};

}  // namespace latticeflow

#endif  // CLICK_DROP_H_
