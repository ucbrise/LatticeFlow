#ifndef PUSHPULL_MAP_H_
#define PUSHPULL_MAP_H_

#include <functional>
#include <map>
#include <utility>
#include <vector>

#include "boost/optional.hpp"

#include "pushpull/lambda.h"
#include "pushpull/phantoms.h"
#include "pushpull/puller.h"
#include "pushpull/pusher.h"

namespace latticeflow {

template <typename Direction, typename Key, typename Left, typename Right>
class Join;

template <typename Key, typename Left, typename Right>
class Join<Push, Key, Left, Right> {
 public:
  explicit Join(Pusher<std::pair<Left, Right>>* downstream,
                std::function<Key(Left)> key_left,
                std::function<Key(Right)> key_right)
      : downstream_(downstream),
        key_left_(key_left),
        key_right_(key_right),
        push_left_([this](Left left) { this->push_left(left); }),
        push_right_([this](Right right) { this->push_right(right); }) {}

  Pusher<Left>& left() { return push_left_; }
  Pusher<Right>& right() { return push_right_; }

 private:
  void push_left(Left left) {
    Key key = key_left_(left);
    left_map_[key].push_back(left);
    for (const Right& right : right_map_[key]) {
      downstream_->push(std::make_pair(left, right));
    }
  }

  void push_right(Right right) {
    Key key = key_right_(right);
    right_map_[key].push_back(right);
    for (const Left& left : left_map_[key]) {
      downstream_->push(std::make_pair(left, right));
    }
  }

  Pusher<std::pair<Left, Right>>* downstream_;

  std::function<Key(Left)> key_left_;
  std::function<Key(Right)> key_right_;

  Lambda<Push, Left> push_left_;
  Lambda<Push, Left> push_right_;

  std::map<Key, std::vector<Left>> left_map_;
  std::map<Key, std::vector<Left>> right_map_;
};

template <typename Key, typename Left, typename Right>
Join<Push, Key, Left, Right> make_join(
    Pusher<std::pair<Left, Right>>* downstream,
    std::function<Key(Left)> key_left, std::function<Key(Right)> key_right) {
  return Join<Push, Key, Left, Right>(downstream, key_left, key_right);
}

template <typename Key, typename Left, typename Right>
class Join<Pull, Key, Left, Right> : public Puller<std::pair<Left, Right>> {
 public:
  explicit Join(Puller<Left>* left, Puller<Right>* right,
                std::function<Key(Left)> key_left,
                std::function<Key(Right)> key_right)
      : left_(left),
        right_(right),
        key_left_(key_left),
        key_right_(key_right) {}

  boost::optional<std::pair<Left, Right>> pull() override {
    if (buf_.size() > 0) {
      std::pair<Left, Right> back = std::move(buf_.back());
      buf_.pop_back();
      return back;
    }

    if (pull_left) {
      boost::optional<Left> left_val = left_->pull();
      if (left_val) {
        Key key = key_left_(*left_val);
        left_map_[key].push_back(*left_val);
        for (const Right& right_val : right_map_[key]) {
          buf_.push_back(std::make_pair(*left_val, right_val));
        }
      } else {
        return {};
      }
    } else {
      boost::optional<Right> right_val = right_->pull();
      if (right_val) {
        Key key = key_right_(*right_val);
        right_map_[key].push_back(*right_val);
        for (const Left& left_val : left_map_[key]) {
          buf_.push_back(std::make_pair(left_val, *right_val));
        }
      } else {
        return {};
      }
    }
    pull_left = !pull_left;

    if (buf_.size() > 0) {
      std::pair<Left, Right> back = std::move(buf_.back());
      buf_.pop_back();
      return back;
    } else {
      return {};
    }
  }

 private:
  Puller<Left>* left_;
  Puller<Right>* right_;
  bool pull_left = true;

  std::function<Key(Left)> key_left_;
  std::function<Key(Right)> key_right_;

  std::map<Key, std::vector<Left>> left_map_;
  std::map<Key, std::vector<Left>> right_map_;

  std::vector<std::pair<Left, Right>> buf_;
};

template <typename Key, typename Left, typename Right>
Join<Pull, Key, Left, Right> make_join(Puller<Left>* left, Puller<Right>* right,
                                       std::function<Key(Left)> key_left,
                                       std::function<Key(Right)> key_right) {
  return Join<Pull, Key, Left, Right>(left, right, key_left, key_right);
}

}  // namespace latticeflow

#endif  // PUSHPULL_MAP_H_
