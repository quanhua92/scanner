
#pragma once

#include "scanner/types.pb.h"
#include "scanner/util/memory.h"

#include <cstddef>
#include <vector>

namespace scanner {
namespace {
template <typename T>
inline T deser(const u8*& buffer, size_t& size_left) {
  assert(size_left >= sizeof(T));
  T e = *((T*)buffer);
  buffer += sizeof(T);
  size_left -= sizeof(T);
  return e;
}
}

template <typename T>
void serialize_proto(const T& element, u8*& buffer, size_t& size) {
  size_t element_size = element.ByteSizeLong();
  buffer = new_buffer(CPU_DEVICE, size);
  size = element_size;
  element.SerializeToArray(buffer, element_size);
}

template <typename T>
T deserialize_proto(const u8* buffer, size_t size) {
  T e;
  e.ParseFromArray(buffer, size);
  return e;
}

template <typename T>
void serialize_proto_vector(const std::vector<T>& elements, u8*& buffer,
                            size_t& size) {
  size = sizeof(size_t);
  for (auto& e : elements) {
    size += e.ByteSizeLong() + sizeof(size_t);
  }
  buffer = new_buffer(CPU_DEVICE, size);

  u8* buf = buffer;
  *((size_t*)buf) = elements.size();
  buf += sizeof(size_t);
  for (size_t i = 0; i < elements.size(); ++i) {
    const T& e = elements[i];
    size_t element_size = e.ByteSizeLong();
    *((size_t*)buf) = element_size;
    buf += sizeof(size_t);
    e.SerializeToArray(buf, element_size);
    buf += element_size;
  }
}

template <typename T>
void serialize_proto_vector_of_vectors(
    const std::vector<std::vector<T>>& elements, u8*& buffer, size_t& size) {
  size = sizeof(size_t);
  for (auto& vec_of_e : elements) {
    size += sizeof(size_t);
    for (auto& e : vec_of_e) {
      size += e.ByteSizeLong() + sizeof(size_t);
    }
  }
  buffer = new_buffer(CPU_DEVICE, size);

  u8* buf = buffer;
  *((size_t*)buf) = elements.size();
  buf += sizeof(size_t);
  for (size_t i = 0; i < elements.size(); ++i) {
    const std::vector<T>& vec_of_e = elements[i];
    *((size_t*)buf) = vec_of_e.size();
    buf += sizeof(size_t);
    for (size_t j = 0; j < vec_of_e.size(); ++j) {
      const T& e = vec_of_e[j];
      size_t element_size = e.ByteSizeLong();
      *((size_t*)buf) = element_size;
      buf += sizeof(size_t);
      e.SerializeToArray(buf, element_size);
      buf += element_size;
    }
  }
}

template <typename T>
std::vector<T> deserialize_proto_vector(const u8* buffer, size_t size) {
  const u8* buf = buffer;
  size_t num_elements = deser<size_t>(buf, size);
  std::vector<T> elements;
  for (size_t i = 0; i < num_elements; ++i) {
    size_t element_size = deser<size_t>(buf, size);
    assert(size >= element_size);
    T e;
    e.ParseFromArray(buf, element_size);
    size -= element_size;
    buf += element_size;
    elements.push_back(e);
  }
  return elements;
}

inline void serialize_bbox_vector(const std::vector<BoundingBox>& bboxes,
                                  u8*& buffer, size_t& size) {
  serialize_proto_vector(bboxes, buffer, size);
}

inline std::vector<BoundingBox> deserialize_bbox_vector(const u8* buffer,
                                                         size_t size) {
  return deserialize_proto_vector<BoundingBox>(buffer, size);
}

// inline void serialize_decode_args(const DecodeArgs& args, u8*& buffer,
//                                   size_t& size) {
//   size = args.ByteSize();
//   buffer = new_buffer(CPU_DEVICE, size);
//   args.SerializeToArray(buffer, size);
// }

// inline DecodeArgs deserialize_decode_args(const u8* buffer, size_t size) {
//   DecodeArgs args;
//   args.ParseFromArray(buffer, size);
//   return args;
// }

// inline void serialize_image_decode_args(const ImageDecodeArgs& args,
//                                         u8*& buffer, size_t& size) {
//   size = args.ByteSize();
//   buffer = new_buffer(CPU_DEVICE, size);
//   args.SerializeToArray(buffer, size);
// }

// inline ImageDecodeArgs deserialize_image_decode_args(const u8* buffer,
//                                                      size_t size) {
//   ImageDecodeArgs args;
//   args.ParseFromArray(buffer, size);
//   return args;
// }
}
