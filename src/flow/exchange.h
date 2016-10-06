#ifndef FLOW_EXCHANGE_H_
#define FLOW_EXCHANGE_H_

#include <mutex>
#include <thread>
#include <tuple>

#include "concurrency/queue.h"
#include "flow/operator.h"

namespace {

enum class Signal {
  RESET = 0,
  DIE = 1,
};

}  // namespace

namespace latticeflow {

template <typename... Ts>
class Exchange : public Operator<Ts...> {
 public:
  Exchange(Operator<Ts...>* const child)
      : child_(child),
        num_resets_(0),
        child_thread_(&Exchange<Ts...>::ChildPopulateBuffer, this) {}

  ~Exchange() {
    signals_.push(Signal::DIE);
    child_thread_.join();
  }

  Exchange(const Exchange&) = delete;
  Exchange& operator=(const Exchange&) = delete;

  boost::optional<std::tuple<Ts...>> next() override {
    auto timestamped_tuple = buffer_.pop();
    while (timestamped_tuple.first != num_resets_) {
      timestamped_tuple = buffer_.pop();
    }
    return timestamped_tuple.second;
  }

  void reset() override {
    num_resets_++;
    signals_.push(Signal::RESET);
  }

 private:
  void ChildPopulateBuffer() {
    int num_resets = 0;

    while (true) {
      for (auto t = child_->next(); t; t = child_->next()) {
        buffer_.push(std::make_pair(num_resets, t));

        boost::optional<Signal> signal = signals_.try_pop();
        if (signal) {
          switch (*signal) {
            case Signal::RESET: {
              buffer_.clear();
              child_->reset();
              num_resets++;
            }
            case Signal::DIE: {
              return;
            }
          }
        }
      }
      buffer_.push(
          std::make_pair(num_resets, boost::optional<std::tuple<Ts...>>()));
      Signal signal = signals_.pop();
      switch (signal) {
        case Signal::RESET: {
          child_->reset();
          num_resets++;
          break;
        }
        case Signal::DIE: {
          return;
        }
      }
    }
  }

  Operator<Ts...>* const child_;
  int num_resets_;
  Queue<std::pair<int, boost::optional<std::tuple<Ts...>>>> buffer_;
  Queue<Signal> signals_;
  std::thread child_thread_;
};

}  // namespace latticeflow

#endif  // FLOW_EXCHANGE_H_
