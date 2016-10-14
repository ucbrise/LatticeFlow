#include "flow/cross.h"

#include <string>
#include <utility>
#include <vector>

#include "boost/optional.hpp"
#include "gtest/gtest.h"

#include "flow/iterator.h"
#include "flow/template_helpers.h"

namespace latticeflow {

TEST(Cross, SimpleCross) {
  using tuple = std::tuple<int, std::string>;
  std::vector<std::tuple<int>> ls = {1, 2, 3};
  std::vector<std::tuple<std::string>> rs = {"a", "b"};
  Iterator<decltype(ls)::iterator, int> l(std::begin(ls), std::end(ls));
  Iterator<decltype(rs)::iterator, std::string> r(std::begin(rs), std::end(rs));
  Cross<left<int>, right<std::string>> crossed(&l, &r);

  EXPECT_EQ(boost::optional<tuple>(std::make_tuple(1, "a")), crossed.next());
  EXPECT_EQ(boost::optional<tuple>(std::make_tuple(1, "b")), crossed.next());
  EXPECT_EQ(boost::optional<tuple>(std::make_tuple(2, "a")), crossed.next());
  EXPECT_EQ(boost::optional<tuple>(std::make_tuple(2, "b")), crossed.next());
  EXPECT_EQ(boost::optional<tuple>(std::make_tuple(3, "a")), crossed.next());
  EXPECT_EQ(boost::optional<tuple>(std::make_tuple(3, "b")), crossed.next());
  EXPECT_EQ(boost::optional<tuple>(), crossed.next());

  crossed.reset();
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
