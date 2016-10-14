#include "lattices/timestamp_lattice.h"

#include <array>

#include "gtest/gtest.h"

#include "lattices/bool_or_lattice.h"
#include "lattices/max_lattice.h"

namespace latticeflow {

using MaxIntLattice = MaxLattice<int>;
using LamportBoolLattice = TimestampLattice<MaxIntLattice, BoolOrLattice>;

TEST(LamportBoolLattice, Basics) {
  BoolOrLattice tru(true);
  BoolOrLattice fls(false);
  LamportBoolLattice x(MaxIntLattice(1), tru);
  LamportBoolLattice y(MaxIntLattice(1), fls);
  LamportBoolLattice z(MaxIntLattice(42), fls);

  EXPECT_EQ(1, x.timestamp().get());
  EXPECT_EQ(1, y.timestamp().get());
  EXPECT_EQ(42, z.timestamp().get());

  EXPECT_EQ(true, x.value().get());
  EXPECT_EQ(false, y.value().get());
  EXPECT_EQ(false, z.value().get());

  y.join(x);
  EXPECT_EQ(1, y.timestamp().get());
  EXPECT_EQ(true, y.value().get());

  y.join(z);
  EXPECT_EQ(42, y.timestamp().get());
  EXPECT_EQ(false, y.value().get());
}

}  // namespace latticeflow

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
