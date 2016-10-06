#include "concurrency/queue.h"

#include "gtest/gtest.h"

namespace latticeflow {

TEST(Queue, SimpleQueue) {
  Queue<int> q;
  q.push(1);
  EXPECT_EQ(1, q.pop());
  q.push(2);
  EXPECT_EQ(2, q.try_pop());
}

TEST(Queue, ComplexQueue) {
  Queue<int> q({1, 2, 3, 4, 5});

  for (int i = 1; i <= 5; ++i) {
    EXPECT_EQ(i, q.pop());
  }

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
