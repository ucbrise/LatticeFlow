#include "click/queue.h"

#include <queue>

#include "boost/optional.hpp"
#include "gtest/gtest.h"

namespace latticeflow {

TEST(Queue, PushPull) {
  Queue<int> q;

  q.push(0);
  q.push(1);
  q.push(2);
  EXPECT_EQ(boost::optional<int>(0), q.pull());
  EXPECT_EQ(boost::optional<int>(1), q.pull());
  q.push(3);
  q.push(4);
  EXPECT_EQ(boost::optional<int>(2), q.pull());
  EXPECT_EQ(boost::optional<int>(3), q.pull());
  EXPECT_EQ(boost::optional<int>(4), q.pull());
  EXPECT_EQ(boost::optional<int>(), q.pull());
}

}  // namespace latticeflow

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
