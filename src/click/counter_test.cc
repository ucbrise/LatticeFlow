#include "click/counter.h"

#include "boost/optional.hpp"
#include "gtest/gtest.h"

#include "click/drop.h"

namespace latticeflow {

TEST(Counter, Push) {
  Drop<Push, int> drop;
  auto counter = make_counter(&drop);

  for (int i = 0; i < 100; ++i) {
    EXPECT_EQ(i, counter.get());
    counter.push(i);
  }
}

TEST(Counter, Pull) {
  Drop<Pull, int> drop;
  auto counter = make_counter(&drop);

  for (int i = 0; i < 100; ++i) {
    EXPECT_EQ(i, counter.get());
    counter.pull();
  }
}

}  // namespace latticeflow

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
