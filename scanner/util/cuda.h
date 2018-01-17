
#pragma once

#ifdef HAVE_CUDA
#include <cuda.h>
#include <cuda_runtime_api.h>
#endif

#include <glog/logging.h>

#ifdef HAVE_CUDA
#define CUDA_PROTECT(s) (s);
#else
#define CUDA_PROTECT(s) \
  { LOG(FATAL) << "Cuda not enabled."; }
#endif

#ifdef HAVE_CUDA

#include <stdio.h>
#include <stdlib.h>
#include <sys/prctl.h>
#include <sys/wait.h>
#include <unistd.h>

#define CU_CHECK(ans) \
  { cuAssert((ans), __FILE__, __LINE__); }

inline void cuAssert(cudaError_t code, const char* file, int line) {
  if (code != cudaSuccess) {
    LOG(FATAL) << "GPUassert: " << cudaGetErrorString(code) << " " << file
               << " " << line;
  }
}

#define CUD_CHECK(ans) \
  { cudAssert((ans), __FILE__, __LINE__); }

inline void cudAssert(CUresult code, const char* file, int line) {
  if (code != CUDA_SUCCESS) {
    const char* err_str;
    cuGetErrorString(code, &err_str);
    LOG(FATAL) << "GPUassert: " << err_str << " " << file << " " << line;
  }
}

#endif
