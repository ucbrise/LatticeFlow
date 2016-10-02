#include "flow/cross.h"

#include <string>
#include <utility>
#include <vector>

#include "boost/optional.hpp"
#include "gtest/gtest.h"

#include "flow/template_helpers.h"
#include "flow/vector.h"

namespace latticeflow {

TEST(Cross, SimpleCross) {
  using tuple = std::tuple<int, std::string>;
  Vector<int> l({1, 2, 3});
  Vector<std::string> r({"a", "b"});
  Cross<left<int>, right<std::string>> crossed(&l, &r);

  EXPECT_EQ(boost::optional<tuple>(std::make_tuple(1, "a")), crossed.next());
  EXPECT_EQ(boost::optional<tuple>(std::make_tuple(1, "b")), crossed.next());
  EXPECT_EQ(boost::optional<tuple>(std::make_tuple(2, "a")), crossed.next());
  EXPECT_EQ(boost::optional<tuple>(std::make_tuple(2, "b")), crossed.next());
  EXPECT_EQ(boost::optional<tuple>(std::make_tuple(3, "a")), crossed.next());
  EXPECT_EQ(boost::optional<tuple>(std::make_tuple(3, "b")), crossed.next());
  EXPECT_EQ(boost::optional<tuple>(), crossed.next());
}

}  // namespace latticeflow

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
