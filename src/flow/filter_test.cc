#include "flow/filter.h"

#include <tuple>
#include <vector>

#include "boost/optional.hpp"
#include "gtest/gtest.h"

#include "flow/iterator.h"

namespace latticeflow {

TEST(Filter, SimpleFilter) {
  using tuple = std::tuple<int>;
  std::vector<tuple> xs = {{1}, {2}, {3}, {4}, {5}, {6}, {7}};
  Iterator<std::vector<tuple>::iterator, int> v(std::begin(xs), std::end(xs));
  Filter<int> evens(&v, [](const tuple& x) { return std::get<0>(x) % 2 == 0; });

  EXPECT_EQ(boost::optional<tuple>(std::make_tuple(2)), evens.next());
  EXPECT_EQ(boost::optional<tuple>(std::make_tuple(4)), evens.next());
  EXPECT_EQ(boost::optional<tuple>(std::make_tuple(6)), evens.next());
  EXPECT_EQ(boost::optional<tuple>(), evens.next());

  evens.reset();
  EXPECT_EQ(boost::optional<tuple>(std::make_tuple(2)), evens.next());
  EXPECT_EQ(boost::optional<tuple>(std::make_tuple(4)), evens.next());
  EXPECT_EQ(boost::optional<tuple>(std::make_tuple(6)), evens.next());
  EXPECT_EQ(boost::optional<tuple>(), evens.next());
}

TEST(Filter, ComplexFilter) {
  using tuple = std::tuple<int, char>;
  std::vector<tuple> xs = {{1, 'a'}, {2, 'b'}, {3, 'c'}, {4, 'd'},
                           {5, 'e'}, {6, 'f'}, {7, 'g'}};
  Iterator<std::vector<tuple>::iterator, int, char> v(std::begin(xs),
                                                      std::end(xs));
  Filter<int, char> filtered(&v, [](const tuple& x) {
    return std::get<0>(x) % 2 == 0 && std::get<1>(x) != 'd';
  });

  EXPECT_EQ(boost::optional<tuple>(std::make_tuple(2, 'b')), filtered.next());
  EXPECT_EQ(boost::optional<tuple>(std::make_tuple(6, 'f')), filtered.next());
  EXPECT_EQ(boost::optional<tuple>(), filtered.next());

  filtered.reset();
  EXPECT_EQ(boost::optional<tuple>(std::make_tuple(2, 'b')), filtered.next());
  EXPECT_EQ(boost::optional<tuple>(std::make_tuple(6, 'f')), filtered.next());
  EXPECT_EQ(boost::optional<tuple>(), filtered.next());
}

}  // namespace latticeflow

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
