#include "pushpull/group_by.h"

#include <functional>
#include <utility>
#include <vector>

#include "boost/optional.hpp"
#include "gtest/gtest.h"

#include "pushpull/id.h"
#include "pushpull/queue.h"
#include "pushpull/register.h"
#include "pushpull/round_robin.h"

namespace latticeflow {

TEST(GroupBy, Push) {
  using T = int;
  using Group = int;
  using Out = std::pair<Group, std::vector<T>>;

  Register<Push, Out> x;
  std::function<Group(T)> group = [](int x) { return x; };
  auto group_by = make_group_by(&x, group);
  auto push_group = make_id(&group_by.push_group());
  auto end_group = make_id(&group_by.end_group());

  {
    end_group.push(42);
    Out out = std::make_pair(42, std::vector<int>({}));
    EXPECT_EQ(out, x.get());
  }

  {
    push_group.push(42);
    push_group.push(42);
    push_group.push(42);
    end_group.push(42);
    Out out = std::make_pair(42, std::vector<int>({42, 42, 42}));
    EXPECT_EQ(out, x.get());
  }

  {
    push_group.push(42);
    push_group.push(1);
    push_group.push(42);
    push_group.push(1);
    push_group.push(42);
    {
      end_group.push(42);
      Out out = std::make_pair(42, std::vector<int>({42, 42, 42}));
      EXPECT_EQ(out, x.get());
    }
    {
      end_group.push(1);
      Out out = std::make_pair(1, std::vector<int>({1, 1}));
      EXPECT_EQ(out, x.get());
    }
  }
}

}  // namespace latticeflow

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
