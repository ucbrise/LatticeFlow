#include "flow/empty.h"

#include <tuple>

#include "boost/optional.hpp"
#include "gtest/gtest.h"

namespace latticeflow {

TEST(Empty, SimpleEmpty) {
  using tuple = std::tuple<int>;
  Empty<int> empty;
  EXPECT_EQ(boost::optional<tuple>(), empty.next());
  empty.reset();
  EXPECT_EQ(boost::optional<tuple>(), empty.next());
}

TEST(Empty, ComplexEmpty) {
  using tuple = std::tuple<int, char>;
  Empty<int, char> empty;
  EXPECT_EQ(boost::optional<tuple>(), empty.next());
  empty.reset();
  EXPECT_EQ(boost::optional<tuple>(), empty.next());
}

}  // namespace latticeflow

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
