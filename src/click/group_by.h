#ifndef CLICK_MAP_H_
#define CLICK_MAP_H_

#include <functional>
#include <map>
#include <utility>
#include <vector>

#include "click/call.h"
#include "click/pushable.h"

namespace latticeflow {

// TODO(mwhittaker): Document.
template <typename Group, typename From, typename To>
class GroupBy {
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
    std::pair<Group, std::vector<From>> p =
        std::make_pair(group, std::move(groups_[group]));
    groups_.erase(group);
    downstream_->push(std::move(p));
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
