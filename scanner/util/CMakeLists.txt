
set(SOURCE_FILES
  common.cpp
  memory.cpp
  profiler.cpp
  fs.cpp
  bbox.cpp
  glog.cpp)

if (OpenCV_FOUND)
  list(APPEND SOURCE_FILES opencv.cpp)
endif()

add_library(util OBJECT
  ${SOURCE_FILES})

if (BUILD_CUDA)
  cuda_add_library(util_cuda
    image.cu)
endif()
