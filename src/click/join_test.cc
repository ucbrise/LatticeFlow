#include "click/join.h"

#include <functional>
#include <utility>

#include "boost/optional.hpp"
#include "gtest/gtest.h"

#include "click/id.h"
#include "click/lambda.h"
#include "click/register.h"

namespace latticeflow {
namespace {

class Nats {
 public:
  boost::optional<int> operator()() {
    x_++;
    return x_;
  }

 private:
  int x_ = -1;
};

class DoubleNats {
 public:
  boost::optional<int> operator()() {
    if (i_ == 0) {
      i_++;
    } else {
      x_++;
      i_ = 0;
    }
    return x_;
  }

 private:
  int x_ = -1;
  int i_ = 1;
};

}  // namespace

TEST(Join, Push) {
  using Key = int;
  using Left = int;
  using Right = int;
  using Out = std::pair<Left, Right>;
  std::function<Key(Left)> key_left = [](int x) { return x; };
  std::function<Key(Left)> key_right = [](int x) { return x; };

  Register<Push, std::pair<Left, Right>> x;
  auto join = make_join(&x, key_left, key_right);
  auto left = make_id(&join.left());
  auto right = make_id(&join.right());

  left.push(42);
  right.push(42);
  EXPECT_EQ(Out(42, 42), x.get());

  left.push(43);
  right.push(43);
  EXPECT_EQ(Out(43, 43), x.get());
}

TEST(Join, Pull) {
  using Key = int;
  using Left = int;
  using Right = int;
  using Out = std::pair<Left, Right>;

  std::function<Key(Left)> key_left = [](int x) { return x; };
  std::function<Key(Left)> key_right = [](int x) { return x; };

  Nats left_nats;
  DoubleNats right_nats;
  Lambda<Pull, Left> left(left_nats);
  Lambda<Pull, Right> right(right_nats);
  auto join = make_join(&left, &right, key_left, key_right);

  EXPECT_EQ(boost::optional<Out>(), join.pull());
  EXPECT_EQ(boost::optional<Out>(std::make_pair(0, 0)), join.pull());
  EXPECT_EQ(boost::optional<Out>(), join.pull());
  EXPECT_EQ(boost::optional<Out>(std::make_pair(0, 0)), join.pull());
  EXPECT_EQ(boost::optional<Out>(), join.pull());
  EXPECT_EQ(boost::optional<Out>(std::make_pair(1, 1)), join.pull());
  EXPECT_EQ(boost::optional<Out>(), join.pull());
  EXPECT_EQ(boost::optional<Out>(std::make_pair(1, 1)), join.pull());
}

}  // namespace latticeflow

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
