#ifndef CONCURRENCY_THREAD_POOL_H_
#define CONCURRENCY_THREAD_POOL_H_

#include <thread>

#include "boost/optional.hpp"

#include "concurrency/queue.h"

namespace latticeflow {

class ThreadPool {
 public:
  explicit ThreadPool(const int num_threads);
  ~ThreadPool();
  ThreadPool(const ThreadPool& barrier) = delete;
  ThreadPool& operator=(const ThreadPool& barrier) = delete;

  void submit(const std::function<void()>& f);

 private:
  void DoWork();

  std::vector<std::thread> threads_;
  Queue<boost::optional<std::function<void()>>> work_;
};

}  // namespace latticeflow

#endif  // CONCURRENCY_THREAD_POOL_H_
