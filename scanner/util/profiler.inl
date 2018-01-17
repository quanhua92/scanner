
#include "scanner/util/profiler.h"

namespace scanner {

///////////////////////////////////////////////////////////////////////////////
/// Profiler
inline void Profiler::add_interval(
  const std::string& key,
  timepoint_t start,
  timepoint_t end)
{
  spin_lock();
  records_.emplace_back(TaskRecord{
    key,
    std::chrono::duration_cast<std::chrono::nanoseconds>(
      start - base_time_).count(),
    std::chrono::duration_cast<std::chrono::nanoseconds>(
      end - base_time_).count()});
  unlock();
}

inline void Profiler::increment(const std::string& key, int64_t value) {
  spin_lock();
  counters_[key] += value;
  unlock();
}

inline void Profiler::spin_lock() {
  while (lock_.test_and_set(std::memory_order_acquire));
}

inline void Profiler::unlock() {
  lock_.clear(std::memory_order_release);
}

}
