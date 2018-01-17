
#pragma once

#include "scanner/eval/evaluator.h"
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
#include <vector>

namespace scanner {

///////////////////////////////////////////////////////////////////////////////
/// IntelVideoDecoder
class IntelVideoDecoder : public VideoDecoder {
 public:
  IntelVideoDecoder(int device_id, DeviceType output_type);

  ~IntelVideoDecoder();

  void configure(const InputFormat& metadata) override;

  bool feed(const u8* encoded_buffer, size_t encoded_size,
            bool discontinuity = false) override;

  bool discard_frame() override;

  bool get_frame(u8* decoded_buffer, size_t decoded_size) override;

  int decoded_frames_buffered() override;

  void wait_until_frames_copied() override;

 private:
  int device_id_;
  DeviceType output_type_;
  AVPacket packet_;
  AVCodec* codec_;
  AVCodecContext* cc_;

  InputFormat metadata_;
  std::vector<u8> conversion_buffer_;
  bool reset_context_;
  SwsContext* sws_context_;

  std::vector<AVFrame*> frame_pool_;
  std::deque<AVFrame*> decoded_frame_queue_;
};
}
