
#pragma once

#include "scanner/api/kernel.h"
#include "scanner/engine/metadata.h"
#include "scanner/util/common.h"
#include "scanner/util/profiler.h"

#include <vector>

namespace scanner {
namespace internal {

enum class VideoEncoderType {
  NVIDIA,
  INTEL,
  SOFTWARE,
};

struct EncodeOptions {
  i32 quality = -1;
  i64 bitrate = -1;
  i64 keyframe_distance = -1;
};

///////////////////////////////////////////////////////////////////////////////
/// VideoEncoder
class VideoEncoder {
 public:
  static std::vector<VideoEncoderType> get_supported_encoder_types();

  static bool has_encoder_type(VideoEncoderType type);

  static VideoEncoder* make_from_config(DeviceHandle device_handle,
                                        i32 num_devices, VideoEncoderType type);

  virtual ~VideoEncoder(){};

  virtual void configure(const FrameInfo& metadata,
                         const EncodeOptions& opts) = 0;

  virtual bool feed(const u8* frame_buffer, size_t frame_size) = 0;

  virtual bool flush() = 0;

  virtual bool get_packet(u8* decoded_buffer, size_t decoded_size,
                          size_t& actual_packet_size) = 0;

  virtual int decoded_packets_buffered() = 0;

  virtual void wait_until_packets_copied() = 0;

  void set_profiler(Profiler* profiler);

 protected:
  Profiler* profiler_ = nullptr;
};
}
}
