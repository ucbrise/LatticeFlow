#include "flow/map.h"

#include <string>
#include <vector>

#include "boost/optional.hpp"
#include "gtest/gtest.h"

#include "flow/template_helpers.h"
#include "flow/vector.h"

namespace latticeflow {

std::tuple<int, std::string> f(const std::tuple<int>& t) {
  int x = std::get<0>(t);
  return std::make_tuple(x, std::to_string(x));
}

TEST(Map, SimpleMap) {
  using tuple = std::tuple<int, std::string>;
  Vector<int> v({1, 2, 3, 4});
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
