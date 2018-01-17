
#pragma once

#include "scanner/api/kernel.h"
#include "scanner/api/op.h"
#include "scanner/util/common.h"

#include <vector>

namespace scanner {

namespace internal {

/**
 * @brief Interface for constructing ops at runtime.
 *
 * Scanner pipelines are composed of a sequence of op factories. A single
 * job may use any number of a given op, so the OpFactory allows
 * the user to capture configuration information about the op (e.g. batch
 * size of a neural net, device type) and pass that information to each new
 * op instance. The OpFactory also provides metadata about
 * the inputs and outputs from the op it produces.
 */
class KernelFactory {
 public:
  KernelFactory(const std::string& op_name, DeviceType type, i32 max_devices,
                const std::map<std::string, DeviceType>& input_devices,
                const std::map<std::string, DeviceType>& output_devices,
                bool can_batch, i32 batch_size, KernelConstructor constructor)
    : op_name_(op_name),
      type_(type),
      max_devices_(max_devices),
      input_devices_(input_devices),
      output_devices_(output_devices),
      can_batch_(can_batch),
      preferred_batch_size_(batch_size),
      constructor_(constructor) {}

  const std::string& get_op_name() const { return op_name_; }

  /** Describes the capabilities of the ops the factory produces. */
  DeviceType get_device_type() const { return type_; }

  i32 get_max_devices() const { return max_devices_; }

  const std::map<std::string, DeviceType>& get_input_devices() const {
    return input_devices_;
  }

  const std::map<std::string, DeviceType>& get_output_devices() const {
    return output_devices_;
  }

  bool can_batch() const { return can_batch_; }

  i32 preferred_batch_size() const { return preferred_batch_size_; }

  /* @brief Constructs a kernel to be used for processing elements of data.
   */
  BaseKernel* new_instance(const KernelConfig& config) {
    return constructor_(config);
  }

 private:
  std::string op_name_;
  DeviceType type_;
  i32 max_devices_;
  std::map<std::string, DeviceType> input_devices_;
  std::map<std::string, DeviceType> output_devices_;
  bool can_batch_;
  i32 preferred_batch_size_;
  KernelConstructor constructor_;
};
}
}
