
#pragma once

#include "scanner/api/kernel.h"
#include "scanner/util/queue.h"
#include "scanner/video/video_encoder.h"

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
/// SoftwareVideoEncoder
class SoftwareVideoEncoder : public VideoEncoder {
 public:
  SoftwareVideoEncoder(i32 device_id, DeviceType output_type, i32 thread_count);

  ~SoftwareVideoEncoder();

  void configure(const FrameInfo& metadata, const EncodeOptions& opts) override;

  bool feed(const u8* frame_buffer, size_t frame_size) override;

  bool flush() override;

  bool get_packet(u8* packet_buffer, size_t packet_size,
                  size_t& actual_packet_size) override;

  int decoded_packets_buffered() override;

  void wait_until_packets_copied() override;

 private:
  void feed_frame(bool flush);

  int device_id_;
  DeviceType output_type_;
  AVCodec* codec_;
  AVCodecContext* cc_;
  AVBitStreamFilterContext* annexb_;

  FrameInfo metadata_;
  i32 frame_width_;
  i32 frame_height_;
  SwsContext* sws_context_;
  bool was_reset_;

  i32 frame_id_;
  AVFrame* frame_;
  Queue<AVPacket*> ready_packet_queue_;
};
}
}
