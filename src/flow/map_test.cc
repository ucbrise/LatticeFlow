#include "flow/map.h"

#include <string>
#include <vector>

#include "boost/optional.hpp"
#include "gtest/gtest.h"

#include "flow/iterator.h"
#include "flow/template_helpers.h"

namespace latticeflow {

std::tuple<int, std::string> f(const std::tuple<int>& t) {
  int x = std::get<0>(t);
  return std::make_tuple(x, std::to_string(x));
}

TEST(Map, SimpleMap) {
  using tuple = std::tuple<int, std::string>;
  std::vector<std::tuple<int>> xs = {{1}, {2}, {3}, {4}};
  Iterator<std::vector<std::tuple<int>>::iterator, int> v(std::begin(xs),
                                                          std::end(xs));
  Map<from<int>, to<int, std::string>> mapped(&v, f);

  EXPECT_EQ(boost::optional<tuple>(std::make_tuple(1, "1")), mapped.next());
  EXPECT_EQ(boost::optional<tuple>(std::make_tuple(2, "2")), mapped.next());
  EXPECT_EQ(boost::optional<tuple>(std::make_tuple(3, "3")), mapped.next());
  EXPECT_EQ(boost::optional<tuple>(std::make_tuple(4, "4")), mapped.next());
  EXPECT_EQ(boost::optional<tuple>(), mapped.next());

  mapped.reset();
  EXPECT_EQ(boost::optional<tuple>(std::make_tuple(1, "1")), mapped.next());
  EXPECT_EQ(boost::optional<tuple>(std::make_tuple(2, "2")), mapped.next());
  EXPECT_EQ(boost::optional<tuple>(std::make_tuple(3, "3")), mapped.next());
  EXPECT_EQ(boost::optional<tuple>(std::make_tuple(4, "4")), mapped.next());
  EXPECT_EQ(boost::optional<tuple>(), mapped.next());
}

}  // namespace latticeflow

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
