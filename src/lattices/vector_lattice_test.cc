#include "lattices/vector_lattice.h"

#include <array>

#include "gtest/gtest.h"

#include "lattices/bool_or_lattice.h"

namespace latticeflow {

using BoolVectorLattice = VectorLattice<BoolOrLattice>;

TEST(BoolVectorLattice, Basics) {
  BoolOrLattice tru(true);
  BoolOrLattice fls(false);
  BoolVectorLattice x({tru, fls, fls});
  BoolVectorLattice y({tru, tru, fls, fls});

  EXPECT_EQ(tru, x.get()[0]);
  EXPECT_EQ(fls, x.get()[1]);
  EXPECT_EQ(fls, x.get()[2]);

  EXPECT_EQ(tru, y.get()[0]);
  EXPECT_EQ(tru, y.get()[1]);
  EXPECT_EQ(fls, y.get()[2]);
  EXPECT_EQ(fls, y.get()[3]);

  x.join(y);
  EXPECT_EQ(tru, x.get()[0]);
  EXPECT_EQ(tru, x.get()[1]);
  EXPECT_EQ(fls, x.get()[2]);
  EXPECT_EQ(fls, x.get()[3]);
}

}  // namespace latticeflow

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
