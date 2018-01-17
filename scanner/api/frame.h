
#pragma once

#include "scanner/metadata.pb.h"
#include "scanner/util/common.h"
#include "scanner/util/profiler.h"

#include <vector>

namespace scanner {

using proto::FrameType;

size_t size_of_frame_type(FrameType type);

const i32 FRAME_DIMS = 3;

//! FrameInfo
struct FrameInfo {
  FrameInfo() = default;
  FrameInfo(const FrameInfo& info) = default;
  FrameInfo(FrameInfo&& info) = default;
  FrameInfo& operator=(const FrameInfo&) = default;

  FrameInfo(int shape0, int shape1, int shape2, FrameType type);
  FrameInfo(const std::vector<int> shapes, FrameType type);

  bool operator==(const FrameInfo& other) const;
  bool operator!=(const FrameInfo& other) const;

  size_t size() const;

  //! Only valid when the dimensions are (height, width, channels)
  int width() const;

  //! Only valid when the dimensions are (height, width, channels)
  int height() const;

  //! Only valid when the dimensions are (height, width, channels)
  int channels() const;

  int shape[FRAME_DIMS];
  FrameType type;
};

//! Frame
class Frame {
 public:
  Frame(FrameInfo info, u8* buffer);

  FrameInfo as_frame_info() const;

  size_t size() const;

  //! Only valid when the dimensions are (height, width, channels)
  int width() const;

  //! Only valid when the dimensions are (height, width, channels)
  int height() const;

  //! Only valid when the dimensions are (height, width, channels)
  int channels() const;

  int shape[FRAME_DIMS];
  FrameType type;
  u8* data;
};

Frame* new_frame(DeviceHandle device, FrameInfo info);

void delete_frame(DeviceHandle device, u8* buffer);

std::vector<Frame*> new_frames(DeviceHandle device, FrameInfo info, i32 num);
}
