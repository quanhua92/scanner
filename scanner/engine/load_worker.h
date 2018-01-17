
#pragma once

#include "scanner/engine/runtime.h"
#include "scanner/engine/video_index_entry.h"
#include "scanner/engine/table_meta_cache.h"
#include "scanner/util/common.h"
#include "scanner/util/queue.h"

namespace scanner {
namespace internal {

struct LoadWorkerArgs {
  // Uniform arguments
  i32 node_id;
  TableMetaCache& table_metadata;
  // Per worker arguments
  int worker_id;
  storehouse::StorageConfig* storage_config;
  Profiler& profiler;
  i32 load_sparsity_threshold;
  i32 io_packet_size;
  i32 work_packet_size;
};

class LoadWorker {
 public:
  LoadWorker(const LoadWorkerArgs& args);

  void feed(LoadWorkEntry& input_entry);

  bool yield(i32 item_size, EvalWorkEntry& output_entry);

  bool done();

 private:
  void read_other_column(i32 table_id, i32 column_id, i32 item_id,
                         i32 item_start, i32 item_end,
                         const std::vector<i64>& rows,
                         ElementList& element_list);
  const i32 node_id_;
  const i32 worker_id_;
  Profiler& profiler_;
  // Setup a distinct storage backend for each IO thread
  std::unique_ptr<storehouse::StorageBackend> storage_;
  // Caching table metadata
  DatabaseMetadata meta_;
  TableMetaCache& table_metadata_;
  // To ammortize opening files
  i32 last_table_id_ = -1;
  std::map<std::tuple<i32, i32, i32>, VideoIndexEntry> index_;
  i32 load_sparsity_threshold_;
  i32 io_packet_size_;
  i32 work_packet_size_;

  // Continuation state
  bool first_item_;
  bool needs_configure_;
  bool needs_reset_;
  LoadWorkEntry entry_;
  i64 current_row_;
  i64 total_rows_;
};

void read_video_column(Profiler& profiler,
                       const VideoIndexEntry& index_entry,
                       const std::vector<i64>& rows, i64 start_offset,
                       ElementList& element_list);
}
}
