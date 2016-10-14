#include "lattices/pair_lattice.h"

#include <utility>

#include "gtest/gtest.h"

#include "lattices/bool_and_lattice.h"
#include "lattices/bool_or_lattice.h"

namespace latticeflow {

using BoolBoolLattice = PairLattice<BoolOrLattice, BoolAndLattice>;

TEST(BoolBoolLattice, Basics) {
  BoolOrLattice x1(true);
  BoolAndLattice y1(false);
  BoolOrLattice x2(false);
  BoolAndLattice y2(true);
  BoolBoolLattice p1(x1, y1);
  BoolBoolLattice p2(x2, y2);

  EXPECT_EQ(true, std::get<0>(p1.get()).get());
  EXPECT_EQ(false, std::get<1>(p1.get()).get());
  EXPECT_EQ(false, std::get<0>(p2.get()).get());
  EXPECT_EQ(true, std::get<1>(p2.get()).get());

  p2.join(p1);
  EXPECT_EQ(true, std::get<0>(p2.get()).get());
  EXPECT_EQ(false, std::get<1>(p2.get()).get());
}

TEST(BoolBoolLattice, Comparisons) {
  BoolOrLattice x1(true);
  BoolAndLattice y1(false);
  BoolOrLattice x2(false);
  BoolAndLattice y2(true);
  BoolBoolLattice p1(x1, y1);
  BoolBoolLattice p2(x2, y2);

  EXPECT_FALSE(p1 == p2);
  EXPECT_TRUE(p1 != p2);
  EXPECT_FALSE(p1 <= p2);
}

}  // namespace latticeflow

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
