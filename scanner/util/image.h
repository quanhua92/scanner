
#pragma once

#include <cstdint>

#ifdef HAVE_CUDA
#include <cuda_runtime.h>
#endif

namespace scanner {

using u8 = uint8_t;

class InputFormat;

#ifdef HAVE_CUDA
cudaError_t convertNV12toRGBA(const u8* in, size_t in_pitch, u8* out,
                              size_t out_pitch, int width, int height,
                              cudaStream_t stream);

cudaError_t convertRGBInterleavedToPlanar(const u8* in, size_t in_pitch,
                                          u8* out, size_t out_pitch, int width,
                                          int height, cudaStream_t stream);
#endif
}
