#ifndef PUSHPULL_PULLER_H_
#define PUSHPULL_PULLER_H_

#include "boost/optional.hpp"

namespace latticeflow {

template <typename T>
class Puller {
 public:
  virtual boost::optional<T> pull() = 0;
};

}  // namespace latticeflow

#endif  // PUSHPULL_PULLER_H_
