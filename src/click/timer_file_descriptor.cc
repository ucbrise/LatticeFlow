#include "click/timer_file_descriptor.h"

#include <sys/timerfd.h>
#include <unistd.h>
#include <cassert>

namespace {

timespec nanoseconds_to_timespec(const std::chrono::nanoseconds d) {
  return {.tv_sec = d / std::chrono::seconds(1),
          .tv_nsec = (d % std::chrono::seconds(1)).count()};
}

}  // namespace

TimerFileDescriptor::TimerFileDescriptor(std::chrono::nanoseconds value,
                                         std::chrono::nanoseconds interval)
    : value_(std::move(value)), interval_(std::move(interval)) {
  // TODO(mwhittaker): Handle errors better.
  assert(value != std::chrono::nanoseconds(0) ||
         interval != std::chrono::nanoseconds(0));
  timerfd_ = timerfd_create(CLOCK_REALTIME, 0 /* flags */);
  assert(timerfd_ != -1);
  const itimerspec new_value = {
      .it_interval = nanoseconds_to_timespec(interval_),
      .it_value = nanoseconds_to_timespec(value_),
  };
  int err = timerfd_settime(timerfd_, 0, &new_value, nullptr);
  assert(err != -1);
}

TimerFileDescriptor::~TimerFileDescriptor() {
  if (timerfd_ != -1) {
    close(timerfd_);
  }
}

TimerFileDescriptor::TimerFileDescriptor(TimerFileDescriptor&& timer)
    : value_(std::move(timer.value_)),
      interval_(std::move(timer.interval_)),
      timerfd_(std::move(timer.timerfd_)) {
  timer.timerfd_ = -1;
}

void swap(TimerFileDescriptor& lhs, TimerFileDescriptor& rhs) {
  std::swap(lhs.value_, rhs.value_);
  std::swap(lhs.interval_, rhs.interval_);
  std::swap(lhs.timerfd_, rhs.timerfd_);
}

TimerFileDescriptor& TimerFileDescriptor::operator=(TimerFileDescriptor timer) {
  swap(*this, timer);
  return *this;
}

uint64_t TimerFileDescriptor::Read() {
  uint64_t num_expirations;
  // TODO(mwhittaker): Error checking.
  int err = read(timerfd_, &num_expirations, sizeof(num_expirations));
  assert(err != -1);
  return num_expirations;
}
