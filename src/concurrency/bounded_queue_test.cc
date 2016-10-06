#include "concurrency/bounded_queue.h"

#include "gtest/gtest.h"

namespace latticeflow {

TEST(BoundedQueue, BoundedSimpleQueue) {
  BoundedQueue<int> q(10);
  q.push(1);
  EXPECT_EQ(1, q.pop());
}

TEST(BoundedQueue, BoundedComplexQueue) {
  BoundedQueue<int> q(100);

  for (int i = 1; i <= 100; ++i) {
    q.push(i);
    EXPECT_EQ(i, q.pop());
  }

  for (int i = 1; i <= 100; ++i) {
    q.push(i);
    EXPECT_EQ(i, q.try_pop());
  }

  for (int i = 1; i <= 100; ++i) {
    q.push(i);
  }
  q.clear();
  for (int i = 1; i <= 100; ++i) {
    q.push(i);
  }
  for (int i = 1; i <= 100; ++i) {
    EXPECT_EQ(i, q.pop());
  }
}

}  // namespace latticeflow

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
