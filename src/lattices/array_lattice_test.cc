#include "lattices/array_lattice.h"

#include <array>

#include "gtest/gtest.h"

#include "lattices/bool_or_lattice.h"

namespace latticeflow {

using BoolArrayLattice = ArrayLattice<BoolOrLattice, 3>;

TEST(BoolArrayLattice, Basics) {
  BoolOrLattice tru(true);
  BoolOrLattice fls(false);
  BoolArrayLattice x(std::array<BoolOrLattice, 3>({{tru, tru, fls}}));
  BoolArrayLattice y(std::array<BoolOrLattice, 3>({{tru, fls, fls}}));

  EXPECT_EQ(tru, std::get<0>(x.get()));
  EXPECT_EQ(tru, std::get<1>(x.get()));
  EXPECT_EQ(fls, std::get<2>(x.get()));
  EXPECT_EQ(tru, std::get<0>(y.get()));
  EXPECT_EQ(fls, std::get<1>(y.get()));
  EXPECT_EQ(fls, std::get<2>(y.get()));

  y.join(x);
  EXPECT_EQ(tru, std::get<0>(y.get()));
  EXPECT_EQ(tru, std::get<1>(y.get()));
  EXPECT_EQ(fls, std::get<2>(y.get()));
}

}  // namespace latticeflow

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
