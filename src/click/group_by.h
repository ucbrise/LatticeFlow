#ifndef CLICK_MAP_H_
#define CLICK_MAP_H_

#include <functional>
#include <map>
#include <utility>
#include <vector>

#include "click/call.h"
#include "click/pushable.h"

namespace latticeflow {

// A `GroupBy` implements the relational GROUP BY operator. Elements annotated
// with their group can be pushed into the `GroupBy` using `GroupBy::push`. The
// contents of a group can be flushed downstream by calling `GroupBy::end`. For
// example,
//
//   Group<int, int, ...> group_by(&downstream);
//   group_by.push().push(std::make_pair(0, 1));
//   group_by.push().push(std::make_pair(1, 100));
//   group_by.push().push(std::make_pair(0, 2));
//   group_by.push().push(std::make_pair(1, 200));
//   group_by.push().push(std::make_pair(0, 3));
//   group_by.end().push(0); // pushes (0, [1, 2, 3])  to downstream
//   group_by.end().push(1); // pushes (1, [100, 200]) to downstream
//   group_by.end().push(0); // pushes (0, [])         to downstream
//   group_by.end().push(1); // pushes (1, [])         to downstream
//   group_by.push().push(std::make_pair(0, 42));
//   group_by.push().push(std::make_pair(1, 14));
//   group_by.end().push(0); // pushes (0, [42])       to downstream
//   group_by.end().push(1); // pushes (1, [14])       to downstream
template <typename Group, typename From, typename To>
class GroupBy {
  // TODO(mwhittaker): Think about whether we actually need to take ownership
  // of groups. I don't think we do.
  static_assert(!std::is_reference<Group>::value,
                "GroupBy takes ownership of group values, so they must be "
                "passed by value.");

  static_assert(
      !std::is_reference<From>::value,
      "GroupBy takes ownership of values, so they must be passed by value.");

  static_assert(
      std::is_convertible<std::pair<Group, std::vector<From>>&&, To>::value,
      "GroupBy moves an std::pair<Group, std::vector<From>> downstream, so it "
      "must be implicitly convertible to something of type To.");

 public:
  explicit GroupBy(Pushable<To>* downstream)
      : downstream_(downstream),
        push_([this](std::pair<Group, From> p) {
          this->push_call(std::move(p));
        }),
        end_([this](Group g) { this->end_call(std::move(g)); }) {}

  Pushable<std::pair<Group, From>&&>& push() { return push_; }
  Pushable<Group&&>& end() { return end_; }

 private:
  void push_call(std::pair<Group, From> x) {
    groups_[std::move(x.first)].push_back(std::move(x.second));
  }

  void end_call(Group group) {
    auto it = groups_.find(group);
    if (it == std::end(groups_)) {
      downstream_->push(std::make_pair(std::move(group), std::vector<From>()));
    } else {
      std::vector<From> xs = std::move(it->second);
      std::pair<Group, std::vector<From>> p =
          std::make_pair(std::move(group), std::move(xs));
      downstream_->push(std::move(p));
      groups_.erase(it);
    }
  }

  Pushable<To>* downstream_;
  using push_call_type = std::function<void(std::pair<Group, From>)>;
  using end_call_type = std::function<void(Group)>;
  Call<std::pair<Group, From>&&, push_call_type> push_;
  Call<Group&&, end_call_type> end_;
  std::map<Group, std::vector<From>> groups_;
};

}  // namespace latticeflow

#endif  // CLICK_MAP_H_
