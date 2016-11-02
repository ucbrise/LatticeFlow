#include "click/tee.h"

#include "boost/optional.hpp"
#include "gtest/gtest.h"

#include "click/register.h"

namespace latticeflow {

TEST(Tee, PushEmpty) {
  Tee<Push, int> tee({});
  tee.push(1);
}

TEST(Tee, Push) {
  Register<Push, int> a;
  Register<Push, int> b;
  Register<Push, int> c;
  a.push(0);
  b.push(0);
  c.push(0);
  auto tee = make_tee<int>({&a, &b, &c});

  tee.push(1);
  EXPECT_EQ(1, a.get());
  EXPECT_EQ(1, b.get());
  EXPECT_EQ(1, c.get());

  tee.push(2);
  EXPECT_EQ(2, a.get());
  EXPECT_EQ(2, b.get());
  EXPECT_EQ(2, c.get());

  tee.push(3);
  EXPECT_EQ(3, a.get());
  EXPECT_EQ(3, b.get());
  EXPECT_EQ(3, c.get());

  tee.push(4);
  EXPECT_EQ(4, a.get());
  EXPECT_EQ(4, b.get());
  EXPECT_EQ(4, c.get());

  tee.push(5);
  EXPECT_EQ(5, a.get());
  EXPECT_EQ(5, b.get());
  EXPECT_EQ(5, c.get());

  tee.push(6);
  EXPECT_EQ(6, a.get());
  EXPECT_EQ(6, b.get());
  EXPECT_EQ(6, c.get());
}

}  // namespace latticeflow

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
