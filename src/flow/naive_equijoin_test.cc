#include "flow/naive_equijoin.h"

#include <string>
#include <utility>
#include <vector>

#include "boost/optional.hpp"
#include "gtest/gtest.h"

#include "flow/template_helpers.h"
#include "flow/vector.h"

namespace latticeflow {

TEST(NaiveEquijoin, SimpleNaiveEquijoin) {
  using tuple = std::tuple<int, bool, int, char>;
  Vector<int, bool> l({{1, true}, {2, false}, {2, true}, {3, true}});
  Vector<int, char> r({{2, 'a'}, {2, 'b'}, {3, 'c'}, {4, 'd'}});
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
