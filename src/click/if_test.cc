#include "click/if.h"

#include "boost/optional.hpp"
#include "gtest/gtest.h"

#include "click/register.h"

namespace latticeflow {

TEST(If, Push) {
  Register<Push, int> tru;
  Register<Push, int> fls;
  std::function<bool(int)> pred = [](int x) { return x % 2 == 0; };
  auto evens = make_if(pred, &tru, &fls);

  evens.push(42);
  EXPECT_EQ(42, tru.get());

  evens.push(43);
  EXPECT_EQ(42, tru.get());
  EXPECT_EQ(43, fls.get());

  evens.push(0);
  EXPECT_EQ(0, tru.get());
  EXPECT_EQ(43, fls.get());

  evens.push(123);
  EXPECT_EQ(0, tru.get());
  EXPECT_EQ(123, fls.get());
}

}  // namespace latticeflow

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
