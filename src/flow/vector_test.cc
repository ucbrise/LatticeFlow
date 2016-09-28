#include "flow/vector.h"

#include <vector>

#include "boost/optional.hpp"
#include "gtest/gtest.h"

namespace latticeflow {

TEST(Vector, EmptyVector) {
  Vector<int> v({});
  EXPECT_EQ(boost::optional<int>(), v.next());

  v.reset();
  EXPECT_EQ(boost::optional<int>(), v.next());
}

TEST(Vector, NonEmptyVector) {
  Vector<int> v({1, 2, 3});
  EXPECT_EQ(boost::optional<int>(1), v.next());
  EXPECT_EQ(boost::optional<int>(2), v.next());
  EXPECT_EQ(boost::optional<int>(3), v.next());
  EXPECT_EQ(boost::optional<int>(), v.next());

  v.reset();
  EXPECT_EQ(boost::optional<int>(1), v.next());
  EXPECT_EQ(boost::optional<int>(2), v.next());
  EXPECT_EQ(boost::optional<int>(3), v.next());
  EXPECT_EQ(boost::optional<int>(), v.next());
}

}  // namespace latticeflow

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
