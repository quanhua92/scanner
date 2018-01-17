
#pragma once

#include "scanner/util/common.h"

#include <cstddef>

namespace scanner {

static const i64 DEFAULT_POOL_SIZE = 2L * 1024L * 1024L * 1024L;

void init_memory_allocators(MemoryPoolConfig config,
                            std::vector<i32> gpu_device_ids);

void destroy_memory_allocators();

u8* new_buffer(DeviceHandle device, size_t size);

u8* new_block_buffer(DeviceHandle device, size_t size, i32 refs);

void add_buffer_ref(DeviceHandle device, u8* buffer);

void add_buffer_refs(DeviceHandle device, u8* buffer, i32 refs);

void delete_buffer(DeviceHandle device, u8* buffer);

void memcpy_buffer(u8* dest_buffer, DeviceHandle dest_device,
                   const u8* src_buffer, DeviceHandle src_device, size_t size);

void memcpy_vec(std::vector<u8*>& dest_buffers, DeviceHandle dest_device,
                const std::vector<u8*>& src_buffers, DeviceHandle src_device,
                const std::vector<size_t>& sizes);

void copy_or_ref_buffers(std::vector<u8*>& dest_buffers,
                         DeviceHandle dest_device,
                         const std::vector<u8*>& src_buffers,
                         DeviceHandle src_device,
                         const std::vector<size_t>& sizes);
}
