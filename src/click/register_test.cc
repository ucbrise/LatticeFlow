#include "click/register.h"

#include "boost/optional.hpp"
#include "gtest/gtest.h"

namespace latticeflow {

TEST(Register, Push) {
  Register<Push, int> x;
  x.push(42);
  EXPECT_EQ(42, x.get());
}

TEST(Register, Pull) {
  Register<Pull, int> x(42);
  EXPECT_EQ(boost::optional<int>(42), x.pull());
}

}  // namespace latticeflow

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
