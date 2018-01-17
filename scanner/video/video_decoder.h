
#pragma once

#include "scanner/api/kernel.h"
#include "scanner/engine/metadata.h"
#include "scanner/util/common.h"
#include "scanner/util/profiler.h"

#include <vector>

namespace scanner {
namespace internal {

class InputFormat;

enum class VideoDecoderType {
  NVIDIA,
  INTEL,
  SOFTWARE,
};

///////////////////////////////////////////////////////////////////////////////
/// VideoDecoder
class VideoDecoder {
 public:
  static std::vector<VideoDecoderType> get_supported_decoder_types();

  static bool has_decoder_type(VideoDecoderType type);

  static VideoDecoder* make_from_config(DeviceHandle device_handle,
                                        i32 num_devices, VideoDecoderType type);

  virtual ~VideoDecoder(){};

  virtual void configure(const FrameInfo& metadata) = 0;

  virtual bool feed(const u8* encoded_buffer, size_t encoded_size,
                    bool discontinuity = false) = 0;

  virtual bool discard_frame() = 0;

  virtual bool get_frame(u8* decoded_buffer, size_t decoded_size) = 0;

  virtual int decoded_frames_buffered() = 0;

  virtual void wait_until_frames_copied() = 0;

  void set_profiler(Profiler* profiler);

 protected:
  Profiler* profiler_ = nullptr;
};
}
}
