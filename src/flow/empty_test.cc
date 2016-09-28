#include "flow/empty.h"

#include "boost/optional.hpp"
#include "gtest/gtest.h"

namespace latticeflow {

TEST(Empty, SimpleEmpty) {
  Empty<int> empty;
  EXPECT_EQ(boost::optional<int>(), empty.next());
  empty.reset();
  EXPECT_EQ(boost::optional<int>(), empty.next());
}

}  // namespace latticeflow

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
