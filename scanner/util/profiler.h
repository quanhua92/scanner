
#pragma once

#include "scanner/util/util.h"

#include <atomic>
#include <fstream>
#include <map>
#include <string>
#include <vector>

namespace storehouse {
class WriteFile;
}

namespace scanner {

class Profiler {
 public:
  Profiler(timepoint_t base_time);

  Profiler(const Profiler& other);

  void add_interval(const std::string& key, timepoint_t start, timepoint_t end);

  void increment(const std::string& key, int64_t value);

  struct TaskRecord {
    std::string key;
    int64_t start;
    int64_t end;
  };

  const std::vector<TaskRecord>& get_records() const;

  const std::map<std::string, int64_t>& get_counters() const;

 protected:
  void spin_lock();
  void unlock();

  timepoint_t base_time_;
  std::atomic_flag lock_;
  std::vector<TaskRecord> records_;
  std::map<std::string, int64_t> counters_;
};

void write_profiler_to_file(storehouse::WriteFile* file, int64_t node,
                            std::string type_name, std::string tag,
                            int64_t worker_num, const Profiler& profiler);

}  // namespace scanner

#include "scanner/util/profiler.inl"
