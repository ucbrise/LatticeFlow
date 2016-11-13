#ifndef CLICK_TIMER_FILE_DESCRIPTOR_H_
#define CLICK_TIMER_FILE_DESCRIPTOR_H_

#include <chrono>
#include <cstdint>

// The `timerfd_create`, `timerfd_settime`, and `timerfd_gettime` system calls
// [1] (included in "sys/timerfd.h") allow us to create one-off and periodic
// timers which signal timer expiration through a file descriptor. Since the
// timers send notifications through file descriptors, they can be polled. See
// [2] for example usage.  A `TimerFileDescriptor` constructs and uniquely owns
// a timer file descriptor.
//
// [1]: http://bit.ly/2f4g8Nn
// [2]: http://bit.ly/2fO3gwj
class TimerFileDescriptor {
 public:
  // Constructs a timer which expires after `value` nanoseconds and then
  // expires every `interval` seconds. One of `value` or `interval` must be
  // non-zero. If `interval` is 0, then the timer fires once. If `value` is 0,
  // the timer fires for the first time after `interval` nanoseconds.
  TimerFileDescriptor(std::chrono::nanoseconds value,
                      std::chrono::nanoseconds interval);

  // Close the file descriptor.
  ~TimerFileDescriptor();

  // File descriptors are uniquely owned; they can be moved but not copied.
  TimerFileDescriptor(const TimerFileDescriptor& timer) = delete;
  TimerFileDescriptor(TimerFileDescriptor&& timer);

  // Assignment operator implemented using the copy-and-swap idiom [1].
  // [1]: http://stackoverflow.com/a/3279550/3187068
  friend void swap(TimerFileDescriptor& lhs, TimerFileDescriptor& rhs);
  TimerFileDescriptor& operator=(TimerFileDescriptor timer);

  // Return the underlying value, interval, or timer file descriptor.
  const std::chrono::nanoseconds& value() const { return value_; }
  const std::chrono::nanoseconds& interval() const { return interval_; }
  int timerfd() const { return timerfd_; }

  // Read from the file descriptor the number of times the timer has expired.
  // This can only be called after the timer has expired. If you're polling the
  // timer, you must call this after the event is triggered. If you don't the
  // event will stay triggered.
  uint64_t Read();

 private:
  std::chrono::nanoseconds value_;
  std::chrono::nanoseconds interval_;
  int timerfd_;
};

#endif  // CLICK_TIMER_FILE_DESCRIPTOR_H_
