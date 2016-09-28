#include "flow/filter.h"

#include <vector>

#include "boost/optional.hpp"
#include "gtest/gtest.h"

#include "flow/vector.h"

namespace latticeflow {

TEST(Filter, SimpleFilter) {
  Vector<int> v({1, 2, 3, 4, 5, 6, 7});
  Filter<int> evens(&v, [](const int& x) { return x % 2 == 0; });
  EXPECT_EQ(boost::optional<int>(2), evens.next());
  EXPECT_EQ(boost::optional<int>(4), evens.next());
  EXPECT_EQ(boost::optional<int>(6), evens.next());
  EXPECT_EQ(boost::optional<int>(), evens.next());
}

}  // namespace latticeflow

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
