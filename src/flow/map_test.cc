#include "flow/map.h"

#include <string>
#include <vector>

#include "boost/optional.hpp"
#include "gtest/gtest.h"

#include "flow/vector.h"

namespace latticeflow {

std::string f(const int& x) { return "foo" + std::to_string(x * 2); }

TEST(Map, SimpleMap) {
  Vector<int> v({1, 2, 3, 4});
  Map<int, std::string> mapped(&v, f);
  EXPECT_EQ(boost::optional<std::string>("foo2"), mapped.next());
  EXPECT_EQ(boost::optional<std::string>("foo4"), mapped.next());
  EXPECT_EQ(boost::optional<std::string>("foo6"), mapped.next());
  EXPECT_EQ(boost::optional<std::string>("foo8"), mapped.next());
  EXPECT_EQ(boost::optional<std::string>(), mapped.next());

  mapped.reset();
  EXPECT_EQ(boost::optional<std::string>("foo2"), mapped.next());
  EXPECT_EQ(boost::optional<std::string>("foo4"), mapped.next());
  EXPECT_EQ(boost::optional<std::string>("foo6"), mapped.next());
  EXPECT_EQ(boost::optional<std::string>("foo8"), mapped.next());
  EXPECT_EQ(boost::optional<std::string>(), mapped.next());
}

}  // namespace latticeflow

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
