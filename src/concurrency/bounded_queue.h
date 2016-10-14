#ifndef CONCURRENCY_BOUNDED_QUEUE_H_
#define CONCURRENCY_BOUNDED_QUEUE_H_

#include <condition_variable>
#include <mutex>
#include <vector>

#include "boost/optional.hpp"

template <typename T>
class BoundedQueue {
 public:
  BoundedQueue(std::size_t capacity) : capacity_(capacity) {
    // TODO(mwhittaker): Assert that capacity_ > 0;
  }
  BoundedQueue(const BoundedQueue&) = delete;
  BoundedQueue& operator=(const BoundedQueue&) = delete;

  void push(const T& x) {
    std::unique_lock<std::mutex> l(m_);
    while (xs_.size() == capacity_) {
      space_available_.wait(l);
    }
    xs_.push_back(x);
    data_available_.notify_one();
  }

  T pop() {
    std::unique_lock<std::mutex> l(m_);
    while (xs_.size() == 0) {
      data_available_.wait(l);
    }
    const T x = xs_.front();
    xs_.erase(std::begin(xs_));
    space_available_.notify_one();
    return x;
  }

  boost::optional<T> try_pop() {
    std::unique_lock<std::mutex> l(m_);
    if (xs_.size() > 0) {
      const T x = xs_.front();
      xs_.erase(std::begin(xs_));
      space_available_.notify_one();
      return x;
    } else {
      return {};
    }
  }

  void clear() {
    std::unique_lock<std::mutex> l(m_);
    xs_.clear();
    space_available_.notify_all();
  }

 private:
  std::vector<T> xs_;
  const std::size_t capacity_;
  std::mutex m_;
  std::condition_variable data_available_;
  std::condition_variable space_available_;
};

#endif  // CONCURRENCY_BOUNDED_QUEUE_H_
