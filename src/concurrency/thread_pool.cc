#include "concurrency/thread_pool.h"

namespace latticeflow {

ThreadPool::ThreadPool(const int num_threads) {
  for (int i = 0; i < num_threads; ++i) {
    threads_.push_back(std::thread(&ThreadPool::DoWork, this));
  }
}

ThreadPool::~ThreadPool() {
  for (int i = 0; i < threads_.size(); ++i) {
    work_.push({});
  }
  for (std::thread& t : threads_) {
    t.join();
  }
}

void ThreadPool::submit(const std::function<void()>& f) { work_.push(f); }

void ThreadPool::DoWork() {
  for (boost::optional<std::function<void()>> f = work_.pop(); f;
       f = work_.pop()) {
    (*f)();
  }
}

}  // namespace latticeflow
