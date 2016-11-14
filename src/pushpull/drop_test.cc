#include "pushpull/drop.h"

#include "boost/optional.hpp"
#include "gtest/gtest.h"

namespace latticeflow {

TEST(Drop, Push) {
  Drop<Push, int> drop;
  drop.push(1);
}

TEST(Drop, Pull) {
  Drop<Pull, int> drop;
  EXPECT_EQ(boost::optional<int>(), drop.pull());
}

}  // namespace latticeflow

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
