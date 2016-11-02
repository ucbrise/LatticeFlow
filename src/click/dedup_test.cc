#include "click/dedup.h"

#include "boost/optional.hpp"
#include "gtest/gtest.h"

#include "click/queue.h"
#include "click/register.h"
#include "click/round_robin.h"

namespace latticeflow {

TEST(Dup, Push) {
  Register<Push, int> x;
  x.push(0);
  auto dedup = make_dedup(&x, 3);

  dedup.push(42);
  EXPECT_EQ(0, x.get());
  dedup.push(42);
  EXPECT_EQ(0, x.get());
  dedup.push(42);
  EXPECT_EQ(42, x.get());

  dedup.push(1);
  EXPECT_EQ(42, x.get());
  dedup.push(2);
  EXPECT_EQ(42, x.get());
  dedup.push(1);
  EXPECT_EQ(42, x.get());
  dedup.push(2);
  EXPECT_EQ(42, x.get());
  dedup.push(1);
  EXPECT_EQ(1, x.get());
  dedup.push(2);
  EXPECT_EQ(2, x.get());
}

TEST(Dup, Pull) {
  Register<Pull, int> x(42);
  Register<Pull, int> y(76);
  auto round_robin = make_round_robin<int>({&x, &y});
  auto dedup = make_dedup(&round_robin, 3);

  EXPECT_EQ(boost::optional<int>(), dedup.pull());
  EXPECT_EQ(boost::optional<int>(), dedup.pull());
  EXPECT_EQ(boost::optional<int>(), dedup.pull());
  EXPECT_EQ(boost::optional<int>(), dedup.pull());
  EXPECT_EQ(boost::optional<int>(42), dedup.pull());
  EXPECT_EQ(boost::optional<int>(76), dedup.pull());
  EXPECT_EQ(boost::optional<int>(), dedup.pull());
  EXPECT_EQ(boost::optional<int>(), dedup.pull());
}

}  // namespace latticeflow

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
