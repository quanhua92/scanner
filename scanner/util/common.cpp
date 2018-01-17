
#include "scanner/util/common.h"

namespace scanner {

std::ostream& operator<<(std::ostream& os, DeviceHandle const& handle) {
  std::string name;
  if (handle.type == DeviceType::CPU) {
    name = "CPU";
  } else if (handle.type == DeviceType::GPU) {
    name = "GPU";
  } else {
    LOG(FATAL) << "Invalid device type";
  }
  return os << "{" << name << ", " << handle.id << "}";
}

StridedInterval::StridedInterval(i32 start, i32 end, i32 stride)
  : start(start), end(end), stride(stride) {}

StridedInterval::StridedInterval(const Interval& i)
  : start(i.start), end(i.end), stride(1) {}

bool string_to_image_encoding_type(const std::string& s,
                                   ImageEncodingType& type) {
  bool success = true;
  if (s == "png" || s == "PNG") {
    type = ImageEncodingType::PNG;
  } else if (s == "jpeg" || s == "JPEG" || s == "jpg" || s == "JPG") {
    type = ImageEncodingType::JPEG;
  } else if (s == "bmp" || s == "BMP") {
    type = ImageEncodingType::BMP;
  } else if (s == "raw" || s == "RAW") {
    type = ImageEncodingType::RAW;
  } else {
    success = false;
  }
  return success;
}

std::string image_encoding_type_to_string(ImageEncodingType t) {
  std::string s;
  switch (t) {
    case ImageEncodingType::JPEG:
      s = "jpeg";
      break;
    case ImageEncodingType::PNG:
      s = "png";
      break;
    case ImageEncodingType::BMP:
      s = "bmp";
      break;
    case ImageEncodingType::RAW:
      s = "raw";
      break;
    default:
      assert(false);
  }
  return s;
}

i32 NUM_CUDA_STREAMS = 32;      // Number of cuda streams for image processing
}
