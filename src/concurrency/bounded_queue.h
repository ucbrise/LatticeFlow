#ifndef CONCURRENCY_BOUNDED_QUEUE_H_
#define CONCURRENCY_BOUNDED_QUEUE_H_

#include <condition_variable>
#include <mutex>
#include <queue>

template <typename T>
class BoundedQueue {
 public:
  BoundedQueue(int capacity) : capacity_(capacity) {}
  BoundedQueue(const BoundedQueue&) = delete;
  BoundedQueue& operator=(const BoundedQueue&) = delete;

  void push(const T& x) {
    std::unique_lock<std::mutex> l(m_);
    while (xs_.size() == capacity_) {
      space_available_.wait(l);
    }
    xs_.push(x);
    data_available_.notify_one();
  }

  T pop() {
    std::unique_lock<std::mutex> l(m_);
    while (xs_.size() == 0) {
      data_available_.wait(l);
    }
    const T& x = xs_.front();
    xs_.pop();
    space_available_.notify_one();
    return x;
  }

 private:
  std::queue<T> xs_;
  const int capacity_;
  std::mutex m_;
  std::condition_variable data_available_;
  std::condition_variable space_available_;
};

#endif  // CONCURRENCY_BOUNDED_QUEUE_H_
