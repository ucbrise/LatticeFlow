#ifndef CLICK_MAP_H_
#define CLICK_MAP_H_

#include <functional>
#include <map>
#include <utility>
#include <vector>

#include "boost/optional.hpp"

#include "click/lambda.h"
#include "click/phantoms.h"
#include "click/puller.h"
#include "click/pusher.h"

namespace latticeflow {

template <typename Group, typename T>
class GroupBy<Push, Group, T> {
 public:
  explicit GroupBy(Pusher<std::pair<Group, std::vector<T>>>* downstream,
                   std::function<Group(T)> group)
      : downstream_(downstream),
        group_(group),
        push_group_([this](T x) { this->push_group(x); }),
        end_group_([this](Group group) { this->end_group(group); }) {}

  Pusher<T>& push_group() { return push_group_; }
  Pusher<Group>& end_group() { return end_group_; }

 private:
  void push_group(T x) {
    Group group = group_(x);
    groups_[group].push_back(x);
  }

  void end_group(Group group) {
    std::pair<Group, std::vector<T>> xs =
        std::make_pair(group, std::move(groups_[group]));
    groups_.erase(group);
    downstream_->push(xs);
  }

  Pusher<std::pair<Group, std::vector<T>>>* downstream_;
  std::function<Group(T)> group_;
  Lambda<Push, T> push_group_;
  Lambda<Push, Group> end_group_;
  std::map<Group, std::vector<T>> groups_;
};

template <typename Group, typename T>
GroupBy<Push, Group, T> make_group_by(
    Pusher<std::pair<Group, std::vector<T>>>* downstream,
    std::function<Group(T)> group) {
  return GroupBy<Push, Group, T>(downstream, group);
}

}  // namespace latticeflow

#endif  // CLICK_MAP_H_
