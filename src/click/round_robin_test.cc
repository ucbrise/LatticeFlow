#include "click/round_robin.h"

#include "boost/optional.hpp"
#include "gtest/gtest.h"

#include "click/register.h"

namespace latticeflow {

TEST(RoundRobin, PushEmpty) {
  RoundRobin<Push, int> round_robin({});
  round_robin.push(1);
}

TEST(RoundRobin, Push) {
  Register<Push, int> a;
  Register<Push, int> b;
  Register<Push, int> c;
  a.push(0);
  b.push(0);
  c.push(0);
  auto round_robin = make_round_robin<int>({&a, &b, &c});

  round_robin.push(1);
  EXPECT_EQ(1, a.get());
  EXPECT_EQ(0, b.get());
  EXPECT_EQ(0, c.get());

  round_robin.push(2);
  EXPECT_EQ(1, a.get());
  EXPECT_EQ(2, b.get());
  EXPECT_EQ(0, c.get());

  round_robin.push(3);
  EXPECT_EQ(1, a.get());
  EXPECT_EQ(2, b.get());
  EXPECT_EQ(3, c.get());

  round_robin.push(4);
  EXPECT_EQ(4, a.get());
  EXPECT_EQ(2, b.get());
  EXPECT_EQ(3, c.get());

  round_robin.push(5);
  EXPECT_EQ(4, a.get());
  EXPECT_EQ(5, b.get());
  EXPECT_EQ(3, c.get());

  round_robin.push(6);
  EXPECT_EQ(4, a.get());
  EXPECT_EQ(5, b.get());
  EXPECT_EQ(6, c.get());
}

TEST(RoundRobin, Pull) {
  Register<Pull, int> a(0);
  Register<Pull, int> b(1);
  Register<Pull, int> c(2);
  auto round_robin = make_round_robin<int>({&a, &b, &c});

  EXPECT_EQ(boost::optional<int>(0), round_robin.pull());
  EXPECT_EQ(boost::optional<int>(1), round_robin.pull());
  EXPECT_EQ(boost::optional<int>(2), round_robin.pull());
  EXPECT_EQ(boost::optional<int>(0), round_robin.pull());
  EXPECT_EQ(boost::optional<int>(1), round_robin.pull());
  EXPECT_EQ(boost::optional<int>(2), round_robin.pull());
}

}  // namespace latticeflow

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
