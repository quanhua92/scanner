
#include "scanner/video/video_encoder.h"

#ifdef HAVE_NVIDIA_VIDEO_HARDWARE
#include "scanner/util/cuda.h"
//#include "scanner/video/nvidia/nvidia_video_encoder.h"
#endif

#ifdef HAVE_INTEL_VIDEO_HARDWARE
#include "scanner/video/intel/intel_video_encoder.h"
#endif

#include "scanner/video/software/software_video_encoder.h"

#include <cassert>

namespace scanner {
namespace internal {

std::vector<VideoEncoderType> VideoEncoder::get_supported_encoder_types() {
  std::vector<VideoEncoderType> encoder_types;
#ifdef HAVE_NVIDIA_VIDEO_HARDWARE
// encoder_types.push_back(VideoEncoderType::NVIDIA);
#endif
#ifdef HAVE_INTEL_VIDEO_HARDWARE
  encoder_types.push_back(VideoEncoderType::INTEL);
#endif
  encoder_types.push_back(VideoEncoderType::SOFTWARE);

  return encoder_types;
}

bool VideoEncoder::has_encoder_type(VideoEncoderType type) {
  std::vector<VideoEncoderType> types =
      VideoEncoder::get_supported_encoder_types();

  for (const VideoEncoderType& supported_type : types) {
    if (type == supported_type) return true;
  }

  return false;
}

VideoEncoder* VideoEncoder::make_from_config(DeviceHandle device_handle,
                                             i32 num_devices,
                                             VideoEncoderType type) {
  VideoEncoder* encoder = nullptr;

  switch (type) {
    case VideoEncoderType::NVIDIA: {
#ifdef HAVE_NVIDIA_VIDEO_HARDWARE
      // HACK: we are just going to assume all processing is done in the
      //   default context for now and retain it ourselves. Ideally we would
      //   allow the user to pass in the CUcontext they want to use for
      //   decoding frames into but that would require providing opaque
      //   configuration data to this function which we are avoiding for now.
      //   The
      //   reason we are avoding it for now is that by having configuration data
      //   for different encoders, the client code ends up needing to do
      //   conditional includes depending on which encoders are available in
      //   order to fill in the configuration data, which is just messy.
      CUD_CHECK(cuInit(0));
      CUcontext cuda_context;
      CUD_CHECK(cuDevicePrimaryCtxRetain(&cuda_context, device_handle.id));

// encoder = new NVIDIAVideoEncoder(device_handle.id, device_handle.type,
//                                  cuda_context);
#else
#endif
      break;
    }
    case VideoEncoderType::INTEL: {
#ifdef HAVE_INTEL_VIDEO_HARDWARE
      encoder = new IntelVideoEncoder(device_handle.id, device_handle.type);
#else
#endif
      break;
    }
    case VideoEncoderType::SOFTWARE: {
      encoder = new SoftwareVideoEncoder(device_handle.id, device_handle.type,
                                         num_devices);
      break;
    }
    default: {}
  }

  return encoder;
}

void VideoEncoder::set_profiler(Profiler* profiler) { profiler_ = profiler; }
}
}
