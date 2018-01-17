
#pragma once

#include "scanner/engine/runtime.h"
#include "scanner/util/common.h"
#include "scanner/util/queue.h"
#include "scanner/util/storehouse.h"

namespace scanner {
namespace internal {

struct SaveWorkerArgs {
  // Uniform arguments
  i32 node_id;

  // Per worker arguments
  int worker_id;
  storehouse::StorageConfig* storage_config;
  Profiler& profiler;
};

class SaveWorker {
 public:
  SaveWorker(const SaveWorkerArgs& args);
  ~SaveWorker();

  void feed(EvalWorkEntry& input_entry);

  void new_task(i32 table_id, i32 task_id,
                std::vector<ColumnType> column_types);

 private:
  const i32 node_id_;
  const i32 worker_id_;
  Profiler& profiler_;
  // Setup a distinct storage backend for each IO thread
  std::unique_ptr<storehouse::StorageBackend> storage_;
  // Files to write io packets to
  std::vector<std::unique_ptr<storehouse::WriteFile>> output_;
  std::vector<std::unique_ptr<storehouse::WriteFile>> output_metadata_;
  std::vector<VideoMetadata> video_metadata_;

  // Continuation state
  bool first_item_;
  bool needs_configure_;
  bool needs_reset_;

  i64 current_work_item_;
  i64 current_row_;
  i64 total_work_items_;

};

}
}
