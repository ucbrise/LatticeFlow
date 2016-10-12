#include "flow/iterator.h"

#include <iterator>
#include <set>
#include <vector>

#include "boost/optional.hpp"
#include "gtest/gtest.h"

namespace latticeflow {

TEST(Iterator, ComplexVectorIterator) {
  using tuple = std::tuple<int, char>;
  std::vector<tuple> v = {{1, 'a'}, {2, 'b'}, {3, 'c'}};
  Iterator<std::vector<tuple>::iterator, int, char> it(std::begin(v),
                                                       std::end(v));

  EXPECT_EQ(boost::optional<tuple>(std::make_tuple(1, 'a')), it.next());
  EXPECT_EQ(boost::optional<tuple>(std::make_tuple(2, 'b')), it.next());
  EXPECT_EQ(boost::optional<tuple>(std::make_tuple(3, 'c')), it.next());
  EXPECT_EQ(boost::optional<tuple>(), it.next());

  it.reset();
  EXPECT_EQ(boost::optional<tuple>(std::make_tuple(1, 'a')), it.next());
  EXPECT_EQ(boost::optional<tuple>(std::make_tuple(2, 'b')), it.next());
  EXPECT_EQ(boost::optional<tuple>(std::make_tuple(3, 'c')), it.next());
  EXPECT_EQ(boost::optional<tuple>(), it.next());
}

TEST(Iterator, ComplexSetIterator) {
  using tuple = std::tuple<int, char>;
  std::set<tuple> s = {{1, 'a'}, {2, 'b'}, {3, 'c'}};
  Iterator<std::set<tuple>::iterator, int, char> it(std::begin(s), std::end(s));

  EXPECT_EQ(boost::optional<tuple>(std::make_tuple(1, 'a')), it.next());
  EXPECT_EQ(boost::optional<tuple>(std::make_tuple(2, 'b')), it.next());
  EXPECT_EQ(boost::optional<tuple>(std::make_tuple(3, 'c')), it.next());
  EXPECT_EQ(boost::optional<tuple>(), it.next());

  it.reset();
  EXPECT_EQ(boost::optional<tuple>(std::make_tuple(1, 'a')), it.next());
  EXPECT_EQ(boost::optional<tuple>(std::make_tuple(2, 'b')), it.next());
  EXPECT_EQ(boost::optional<tuple>(std::make_tuple(3, 'c')), it.next());
  EXPECT_EQ(boost::optional<tuple>(), it.next());
}

}  // namespace latticeflow

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
