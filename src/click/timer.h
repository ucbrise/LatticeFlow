#ifndef CLICK_TIMER_H_
#define CLICK_TIMER_H_

#include <chrono>
#include <map>
#include <type_traits>
#include <utility>

#include "click/driver.h"
#include "click/event_handler.h"
#include "click/pushable.h"
#include "click/timer_file_descriptor.h"

namespace latticeflow {

// `TimerEventHandler`s are created by `Timer`s and push values downstream
// after a delay.
template <typename From, typename To>
class TimerEventHandler : public EventHandler {
  static_assert(!std::is_reference<From>::value,
                "Timers take ownership of values pushed to them. Thus, they "
                "must take arguments by value. If you would like a timer "
                "object to not take ownership of an object, pass in a "
                "pointer.");

 public:
  // Constructs a `TimerEventHandler` that pushes `x` downstream to
  // `downstream` after `delay` nanoseconds.
  //
  // The lifetime of `TimerEventHandler` is rather complicated. `Timer`s create
  // `TimerEventHandlers` dynamically, storing pointers to them in
  // `timer_event_handlers` keyed by unique ids. This `TimerEventHandler` lives
  // in `timer_event_handlers` and is keyed by `timer_id`. After `Run` is run,
  // this object removes itself from the map and frees itself.
  TimerEventHandler(const std::chrono::nanoseconds delay, From&& x,
                    Pushable<To>* downstream, const int timer_id,
                    std::map<int, std::unique_ptr<TimerEventHandler<From, To>>>*
                        timer_event_handlers)
      : x_(std::move(x)),
        downstream_(downstream),
        timer_id_(timer_id),
        timer_event_handlers_(timer_event_handlers),
        timerfd_(delay, std::chrono::nanoseconds(0)) {}

  bool IsOneOff() const override { return true; }

  zmq::pollitem_t GetPollitem() override {
    return {nullptr, timerfd_.timerfd(), ZMQ_POLLIN, 0};
  }

  void Run(zmq::pollitem_t) override {
    downstream_->push(std::move(x_));

    // Transfer ownsership from the map to here.
    std::unique_ptr<TimerEventHandler<From, To>> me(
        timer_event_handlers_->find(timer_id_)->second.release());

    // Now that we disarmed the unique_ptr in timer_event_handlers_, we can
    // safely remove it.
    timer_event_handlers_->erase(timer_id_);

    // When this function returns, me's destructor will be called freing this
    // object. Note that killing ourselves like this is legal [1], albeit super
    // duper confusing.
    //
    // TODO(mwhittaker): Is there a less complex way to manage the lifetimes of
    // these event handlers?
    //
    // [1]: http://stackoverflow.com/a/3150965/3187068
  }

 private:
  From x_;
  Pushable<To>* downstream_;
  const int timer_id_;
  std::map<int, std::unique_ptr<TimerEventHandler<From, To>>>*
      timer_event_handlers_;
  TimerFileDescriptor timerfd_;
};

// `Timer(d, k, a).push(x)` pushes `x` to `a` after `k` nanoseconds. This delay
// is not implemented using blocking. Instead, a new `EventHandler` is
// registered with `d` which awakes `k` nanoseconds in the future.
template <typename From, typename To>
class Timer : public Pushable<From> {
  static_assert(!std::is_reference<From>::value,
                "Timers take ownership of values pushed to them. Thus, they "
                "must take arguments by value. If you would like a timer "
                "object to not take ownership of an object, pass in a "
                "pointer.");

 public:
  explicit Timer(Driver* driver, std::chrono::nanoseconds delay,
                 Pushable<To>* downstream)
      : driver_(driver), delay_(delay), downstream_(downstream), timer_id_(0) {}

  void push(From x) override {
    std::unique_ptr<TimerEventHandler<From, To>> t(
        new TimerEventHandler<From, To>(delay_, std::move(x), downstream_,
                                        timer_id_, &timer_event_handlers_));
    timer_event_handlers_.insert(std::make_pair(timer_id_, std::move(t)));
    driver_->RegisterEventHandler(timer_event_handlers_[timer_id_].get());
    timer_id_++;
  }

 private:
  // The driver with which we register `TimerEventHandler`s.
  Driver* driver_;

  // The delay between receiving a value and pushing it downstream.
  std::chrono::nanoseconds delay_;

  // The downstream `Pushable` to which we push.
  Pushable<To>* downstream_;

  // Each time a value is pushed into a `Timer`, a new `TimerEventHandler` is
  // dynamically allocated, put into a `unique_ptr` and put in
  // `timer_event_handlers_` with a unique id. The current id is maintained in
  // `timer_id_`. When a `TimerEventHandler` is run, it will remove itself from
  // the map and free itself.
  int timer_id_;
  std::map<int, std::unique_ptr<TimerEventHandler<From, To>>>
      timer_event_handlers_;
};

}  // namespace latticeflow

#endif  //  CLICK_TIMER_H_
