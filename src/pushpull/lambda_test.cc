#include "pushpull/lambda.h"

#include "boost/optional.hpp"
#include "gtest/gtest.h"

namespace latticeflow {
namespace {

class Nats {
 public:
  boost::optional<int> operator()() {
    x_++;
    return x_;
  }

 private:
  int x_ = -1;
};

}  // namespace

TEST(Lambda, Push) {
  int y = 0;
  int* yp = &y;
  Lambda<Push, int> l([yp](int x) { *yp = x; });

  l.push(42);
  EXPECT_EQ(42, y);
  l.push(100);
  EXPECT_EQ(100, y);
}

TEST(Lambda, Pull) {
  Nats n;
  Lambda<Pull, int> l(n);
  EXPECT_EQ(boost::optional<int>(0), l.pull());
  EXPECT_EQ(boost::optional<int>(1), l.pull());
  EXPECT_EQ(boost::optional<int>(2), l.pull());
}

}  // namespace latticeflow

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
