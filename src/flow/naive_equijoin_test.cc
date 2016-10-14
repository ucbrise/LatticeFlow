#include "flow/naive_equijoin.h"

#include <string>
#include <utility>
#include <vector>

#include "boost/optional.hpp"
#include "gtest/gtest.h"

#include "flow/iterator.h"
#include "flow/template_helpers.h"

namespace latticeflow {

TEST(NaiveEquijoin, SimpleNaiveEquijoin) {
  using tuple = std::tuple<int, bool, int, char>;
  std::vector<std::tuple<int, bool>> ls = {
      {1, true}, {2, false}, {2, true}, {3, true}};
  std::vector<std::tuple<int, char>> rs = {
      {2, 'a'}, {2, 'b'}, {3, 'c'}, {4, 'd'}};
  Iterator<decltype(ls)::iterator, int, bool> l(std::begin(ls), std::end(ls));
  Iterator<decltype(rs)::iterator, int, char> r(std::begin(rs), std::end(rs));
  NaiveEquijoin<left<int, bool>, right<int, char>, 0, 0> joined(&l, &r);

  EXPECT_EQ(boost::optional<tuple>(std::make_tuple(2, false, 2, 'a')),
            joined.next());
  EXPECT_EQ(boost::optional<tuple>(std::make_tuple(2, false, 2, 'b')),
            joined.next());
  EXPECT_EQ(boost::optional<tuple>(std::make_tuple(2, true, 2, 'a')),
            joined.next());
  EXPECT_EQ(boost::optional<tuple>(std::make_tuple(2, true, 2, 'b')),
            joined.next());
  EXPECT_EQ(boost::optional<tuple>(std::make_tuple(3, true, 3, 'c')),
            joined.next());
  EXPECT_EQ(boost::optional<tuple>(), joined.next());

  joined.reset();
  EXPECT_EQ(boost::optional<tuple>(std::make_tuple(2, false, 2, 'a')),
            joined.next());
  EXPECT_EQ(boost::optional<tuple>(std::make_tuple(2, false, 2, 'b')),
            joined.next());
  EXPECT_EQ(boost::optional<tuple>(std::make_tuple(2, true, 2, 'a')),
            joined.next());
  EXPECT_EQ(boost::optional<tuple>(std::make_tuple(2, true, 2, 'b')),
            joined.next());
  EXPECT_EQ(boost::optional<tuple>(std::make_tuple(3, true, 3, 'c')),
            joined.next());
  EXPECT_EQ(boost::optional<tuple>(), joined.next());
}

}  // namespace latticeflow

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
