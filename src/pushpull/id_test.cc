#include "pushpull/id.h"

#include "boost/optional.hpp"
#include "gtest/gtest.h"

#include "pushpull/drop.h"

namespace latticeflow {

TEST(Id, Push) {
  Drop<Push, int> drop;
  auto id = make_id(&drop);
  id.push(42);
}

TEST(Id, Pull) {
  Drop<Pull, int> drop;
  auto id = make_id(&drop);
  EXPECT_EQ(boost::optional<int>(), id.pull());
}

}  // namespace latticeflow

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
