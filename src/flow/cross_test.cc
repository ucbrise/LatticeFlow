#include "flow/cross.h"

#include <string>
#include <utility>
#include <vector>

#include "boost/optional.hpp"
#include "gtest/gtest.h"

#include "flow/vector.h"

namespace latticeflow {

using optional_pair = boost::optional<std::pair<int, std::string>>;

TEST(Cross, SimpleCross) {
  Vector<int> left({1, 2, 3});
  Vector<std::string> right({"a", "b"});
  Cross<int, std::string> crossed(&left, &right);

  EXPECT_EQ(optional_pair(std::make_pair(1, "a")), crossed.next());
  EXPECT_EQ(optional_pair(std::make_pair(1, "b")), crossed.next());
  EXPECT_EQ(optional_pair(std::make_pair(2, "a")), crossed.next());
  EXPECT_EQ(optional_pair(std::make_pair(2, "b")), crossed.next());
  EXPECT_EQ(optional_pair(std::make_pair(3, "a")), crossed.next());
  EXPECT_EQ(optional_pair(std::make_pair(3, "b")), crossed.next());
  EXPECT_EQ(optional_pair(), crossed.next());

  crossed.reset();
  EXPECT_EQ(optional_pair(std::make_pair(1, "a")), crossed.next());
  EXPECT_EQ(optional_pair(std::make_pair(1, "b")), crossed.next());
  EXPECT_EQ(optional_pair(std::make_pair(2, "a")), crossed.next());
  EXPECT_EQ(optional_pair(std::make_pair(2, "b")), crossed.next());
  EXPECT_EQ(optional_pair(std::make_pair(3, "a")), crossed.next());
  EXPECT_EQ(optional_pair(std::make_pair(3, "b")), crossed.next());
  EXPECT_EQ(optional_pair(), crossed.next());
}

}  // namespace latticeflow

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
