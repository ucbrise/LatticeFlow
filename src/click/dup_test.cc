#include "click/dup.h"

#include "boost/optional.hpp"
#include "gtest/gtest.h"

#include "click/queue.h"
#include "click/register.h"
#include "click/round_robin.h"

namespace latticeflow {

TEST(Dup, Push) {
  Queue<int> q;
  auto dup = make_dup((Pusher<int> *)&q, 3);

  dup.push(42);
  EXPECT_EQ(boost::optional<int>(42), q.pull());
  EXPECT_EQ(boost::optional<int>(42), q.pull());
  EXPECT_EQ(boost::optional<int>(42), q.pull());
  EXPECT_EQ(boost::optional<int>(), q.pull());

  dup.push(76);
  EXPECT_EQ(boost::optional<int>(76), q.pull());
  EXPECT_EQ(boost::optional<int>(76), q.pull());
  EXPECT_EQ(boost::optional<int>(76), q.pull());
  EXPECT_EQ(boost::optional<int>(), q.pull());
}

TEST(Dup, Pull) {
  Register<Pull, int> x(42);
  Register<Pull, int> y(76);
  auto round_robin = make_round_robin<int>({&x, &y});
  auto dup = make_dup(&round_robin, 3);

  EXPECT_EQ(boost::optional<int>(42), dup.pull());
  EXPECT_EQ(boost::optional<int>(42), dup.pull());
  EXPECT_EQ(boost::optional<int>(42), dup.pull());
  EXPECT_EQ(boost::optional<int>(76), dup.pull());
  EXPECT_EQ(boost::optional<int>(76), dup.pull());
  EXPECT_EQ(boost::optional<int>(76), dup.pull());
}

}  // namespace latticeflow

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
