
#pragma once

#include "scanner/api/kernel.h"
#include "scanner/engine/kernel_factory.h"

#include "scanner/util/common.h"

#include <map>

namespace scanner {
namespace internal {

class KernelRegistry {
 public:
  void add_kernel(const std::string& name, KernelFactory* factory);

  bool has_kernel(const std::string& name, DeviceType device_type);

  KernelFactory* get_kernel(const std::string& name, DeviceType device_type);

 protected:
  static std::string factory_name(const std::string& name, DeviceType type);

 private:
  std::map<std::string, KernelFactory*> factories_;
};

KernelRegistry* get_kernel_registry();
}
}
