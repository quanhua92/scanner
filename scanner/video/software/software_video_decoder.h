
#pragma once

#include "scanner/api/kernel.h"
#include "scanner/util/queue.h"
#include "scanner/video/video_decoder.h"

extern "C" {
#include "libavcodec/avcodec.h"
#include "libavfilter/avfilter.h"
#include "libavformat/avformat.h"
#include "libavformat/avio.h"
#include "libavutil/error.h"
#include "libavutil/opt.h"
#include "libavutil/pixdesc.h"
#include "libswscale/swscale.h"
}

#include <deque>
#include <mutex>
#include <vector>

namespace scanner {
namespace internal {

///////////////////////////////////////////////////////////////////////////////
/// SoftwareVideoDecoder
class SoftwareVideoDecoder : public VideoDecoder {
 public:
  SoftwareVideoDecoder(i32 device_id, DeviceType output_type, i32 thread_count);

  ~SoftwareVideoDecoder();

  void configure(const FrameInfo& metadata) override;

  bool feed(const u8* encoded_buffer, size_t encoded_size,
            bool discontinuity = false) override;

  bool discard_frame() override;

  bool get_frame(u8* decoded_buffer, size_t decoded_size) override;

  int decoded_frames_buffered() override;

  void wait_until_frames_copied() override;

 private:
  void feed_packet(bool flush);

  int device_id_;
  DeviceType output_type_;
  AVPacket packet_;
  AVCodec* codec_;
  AVCodecContext* cc_;

  FrameInfo metadata_;
  i32 frame_width_;
  i32 frame_height_;
  std::vector<u8> conversion_buffer_;
  bool reset_context_;
  SwsContext* sws_context_;

  Queue<AVFrame*> frame_pool_;
  Queue<AVFrame*> decoded_frame_queue_;
};
}
}
