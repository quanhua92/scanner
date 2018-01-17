
#pragma once

#include "scanner/engine/metadata.h"
#include "scanner/engine/runtime.h"
#include "scanner/util/common.h"

#include "storehouse/storage_backend.h"

namespace scanner {
namespace internal {

struct VideoIndexEntry {
  std::unique_ptr<storehouse::RandomReadFile> open_file() const;

  storehouse::StorageBackend* storage;
  std::string path;
  bool inplace;
  i32 table_id;
  i32 column_id;
  i32 item_id;
  i32 width;
  i32 height;
  i32 channels;
  FrameType frame_type;
  proto::VideoDescriptor::VideoCodecType codec_type;
  u64 file_size;
  i32 num_encoded_videos;
  std::vector<i64> frames_per_video;
  std::vector<i64> keyframes_per_video;
  std::vector<i64> size_per_video;

  std::vector<u64> keyframe_indices;
  std::vector<u64> sample_offsets;
  std::vector<u64> sample_sizes;
  std::vector<u8> metadata;
};

VideoIndexEntry read_video_index(storehouse::StorageBackend *storage,
                                 i32 table_id, i32 column_id, i32 item_id);

VideoIndexEntry read_video_index(storehouse::StorageBackend *storage,
                                 const VideoMetadata& video_meta);
}
}
