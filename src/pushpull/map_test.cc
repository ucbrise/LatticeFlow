#include "pushpull/map.h"

#include <functional>

#include "boost/optional.hpp"
#include "gtest/gtest.h"

#include "pushpull/register.h"

namespace latticeflow {

TEST(Map, Push) {
  Register<Push, bool> x;
  std::function<bool(int)> f = [](int x) { return x % 2 == 0; };
  auto is_even = make_map(f, &x);

  is_even.push(0);
  EXPECT_EQ(true, x.get());
  is_even.push(1);
  EXPECT_EQ(false, x.get());
  is_even.push(2);
  EXPECT_EQ(true, x.get());
}

TEST(Map, Pull) {
  std::function<bool(int)> f = [](int x) { return x % 2 == 0; };

  {
    Register<Pull, int> x(0);
    auto is_even = make_map(f, &x);
    EXPECT_EQ(boost::optional<bool>(true), is_even.pull());
  }

  {
    Register<Pull, int> x(1);
    auto is_even = make_map(f, &x);
    EXPECT_EQ(boost::optional<bool>(false), is_even.pull());
  }

  {
    Register<Pull, int> x(2);
    auto is_even = make_map(f, &x);
    EXPECT_EQ(boost::optional<bool>(true), is_even.pull());
  }

  {
    Register<Pull, int> x(3);
    auto is_even = make_map(f, &x);
    EXPECT_EQ(boost::optional<bool>(false), is_even.pull());
  }
}

}  // namespace latticeflow

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
