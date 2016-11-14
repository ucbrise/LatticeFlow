#include "pushpull/filter.h"

#include <functional>

#include "boost/optional.hpp"
#include "gtest/gtest.h"

#include "pushpull/register.h"

namespace latticeflow {

TEST(Filter, Push) {
  Register<Push, int> x;
  std::function<bool(int)> pred = [](int x) { return x % 2 == 0; };
  auto evens = make_filter(pred, &x);

  x.push(0);
  evens.push(1);
  EXPECT_EQ(0, x.get());

  evens.push(2);
  EXPECT_EQ(2, x.get());

  evens.push(3);
  EXPECT_EQ(2, x.get());
}

TEST(Filter, Pull) {
  std::function<bool(int)> pred = [](int x) { return x % 2 == 0; };

  {
    Register<Pull, int> x(0);
    auto evens = make_filter(pred, &x);
    EXPECT_EQ(boost::optional<int>(0), evens.pull());
  }

  {
    Register<Pull, int> x(1);
    auto evens = make_filter(pred, &x);
    EXPECT_EQ(boost::optional<int>(), evens.pull());
  }

  {
    Register<Pull, int> x(2);
    auto evens = make_filter(pred, &x);
    EXPECT_EQ(boost::optional<int>(2), evens.pull());
  }

  {
    Register<Pull, int> x(3);
    auto evens = make_filter(pred, &x);
    EXPECT_EQ(boost::optional<int>(), evens.pull());
  }
}

}  // namespace latticeflow

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
