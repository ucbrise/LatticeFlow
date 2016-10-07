#include "concurrency/thread_pool.h"

#include <array>
#include <numeric>

#include "gtest/gtest.h"

namespace latticeflow {

TEST(ThreadPool, SimpleThreadPool) {
  constexpr int num_threads = 100;
  std::array<int, num_threads> xs;

  {
    ThreadPool p(num_threads);
    for (int i = 0; i < num_threads; ++i) {
      int* x = &xs[i];
      p.submit([i, x]() { *x = i * i; });
    }
  }

  int sum = 0;
  for (int i = 0; i < num_threads; ++i) {
    sum += i * i;
  }
  EXPECT_EQ(sum, std::accumulate(std::begin(xs), std::end(xs), 0));
}

}  // namespace latticeflow

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
