#ifndef CONCURRENCY_QUEUE_H_
#define CONCURRENCY_QUEUE_H_

#include <condition_variable>
#include <mutex>
#include <vector>

#include "boost/optional.hpp"

template <typename T>
class Queue {
 public:
  Queue() = default;
  explicit Queue(std::vector<T> xs) : xs_(std::move(xs)) {}
  Queue(const Queue&) = delete;
  Queue& operator=(const Queue&) = delete;

  void push(const T& x) {
    std::unique_lock<std::mutex> l(m_);
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
    return x;
  }

  boost::optional<T> try_pop() {
    std::unique_lock<std::mutex> l(m_);
    if (xs_.size() > 0) {
      const T x = xs_.front();
      xs_.erase(std::begin(xs_));
      return x;
    } else {
      return {};
    }
  }

  void clear() {
    std::unique_lock<std::mutex> l(m_);
    xs_.clear();
  }

 private:
  std::mutex m_;
  std::condition_variable data_available_;
  std::vector<T> xs_;
};

#endif  // CONCURRENCY_QUEUE_H_
