#include "flow/exchange.h"

#include <vector>

#include "boost/optional.hpp"
#include "gtest/gtest.h"

#include "flow/iterator.h"

namespace latticeflow {

TEST(Exchange, SimplExchange) {
  using tuple = std::tuple<int>;
  std::vector<tuple> xs = {{1}, {2}, {3}};
  Iterator<decltype(xs)::iterator, int> v(std::begin(xs), std::end(xs));
  Exchange<int> exchange(&v);

  EXPECT_EQ(boost::optional<tuple>(std::make_tuple(1)), exchange.next());
  EXPECT_EQ(boost::optional<tuple>(std::make_tuple(2)), exchange.next());
  EXPECT_EQ(boost::optional<tuple>(std::make_tuple(3)), exchange.next());
  EXPECT_EQ(boost::optional<tuple>(), exchange.next());

  exchange.reset();
  EXPECT_EQ(boost::optional<tuple>(std::make_tuple(1)), exchange.next());
  EXPECT_EQ(boost::optional<tuple>(std::make_tuple(2)), exchange.next());
  EXPECT_EQ(boost::optional<tuple>(std::make_tuple(3)), exchange.next());
  EXPECT_EQ(boost::optional<tuple>(), exchange.next());
}

}  // namespace latticeflow

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
