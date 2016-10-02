#include "flow/vector.h"

#include <vector>

#include "boost/optional.hpp"
#include "gtest/gtest.h"

namespace latticeflow {

TEST(Vector, SimpleEmptyVector) {
  using tuple = std::tuple<int>;
  Vector<int> v({});
  EXPECT_EQ(boost::optional<tuple>(), v.next());
  v.reset();
  EXPECT_EQ(boost::optional<tuple>(), v.next());
}

TEST(Vector, SimpleNonEmptyVector) {
  using tuple = std::tuple<int>;
  Vector<int> v({{1}, {2}, {3}});

  EXPECT_EQ(boost::optional<tuple>(std::make_tuple(1)), v.next());
  EXPECT_EQ(boost::optional<tuple>(std::make_tuple(2)), v.next());
  EXPECT_EQ(boost::optional<tuple>(std::make_tuple(3)), v.next());
  EXPECT_EQ(boost::optional<tuple>(), v.next());

  v.reset();
  EXPECT_EQ(boost::optional<tuple>(std::make_tuple(1)), v.next());
  EXPECT_EQ(boost::optional<tuple>(std::make_tuple(2)), v.next());
  EXPECT_EQ(boost::optional<tuple>(std::make_tuple(3)), v.next());
  EXPECT_EQ(boost::optional<tuple>(), v.next());
}

TEST(Vector, ComplexEmptyVector) {
  using tuple = std::tuple<int, char>;
  Vector<int, char> v({});
  EXPECT_EQ(boost::optional<tuple>(), v.next());
  v.reset();
  EXPECT_EQ(boost::optional<tuple>(), v.next());
}

TEST(Vector, ComplexNonEmptyVector) {
  using tuple = std::tuple<int, char>;
  Vector<int, char> v({{1, 'a'}, {2, 'b'}, {3, 'c'}});

  EXPECT_EQ(boost::optional<tuple>(std::make_tuple(1, 'a')), v.next());
  EXPECT_EQ(boost::optional<tuple>(std::make_tuple(2, 'b')), v.next());
  EXPECT_EQ(boost::optional<tuple>(std::make_tuple(3, 'c')), v.next());
  EXPECT_EQ(boost::optional<tuple>(), v.next());

  v.reset();
  EXPECT_EQ(boost::optional<tuple>(std::make_tuple(1, 'a')), v.next());
  EXPECT_EQ(boost::optional<tuple>(std::make_tuple(2, 'b')), v.next());
  EXPECT_EQ(boost::optional<tuple>(std::make_tuple(3, 'c')), v.next());
  EXPECT_EQ(boost::optional<tuple>(), v.next());
}

}  // namespace latticeflow

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
