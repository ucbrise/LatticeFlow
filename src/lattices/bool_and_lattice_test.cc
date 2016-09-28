#include "lattices/bool_and_lattice.h"

#include "gtest/gtest.h"

namespace latticeflow {

TEST(BoolAndLattice, Basics) {
  BoolAndLattice x(true);
  BoolAndLattice y(false);
  EXPECT_EQ(true, x.get());
  EXPECT_EQ(false, y.get());

  y.join(x);
  x.join(y);
  EXPECT_EQ(false, x.get());
  EXPECT_EQ(false, y.get());
}

TEST(BoolAndLattice, Comparison) {
  BoolAndLattice tru(true);
  BoolAndLattice fls(false);

  EXPECT_TRUE(tru < fls);
  EXPECT_TRUE(tru <= fls);
  EXPECT_FALSE(tru > fls);
  EXPECT_FALSE(tru >= fls);
  EXPECT_FALSE(tru == fls);
  EXPECT_TRUE(tru != fls);
}

}  // namespace latticeflow

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
