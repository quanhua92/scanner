
#include "scanner/engine/kernel_registry.h"

namespace scanner {
namespace internal {

void KernelRegistry::add_kernel(const std::string& name,
                                KernelFactory* factory) {
  DeviceType type = factory->get_device_type();
  factories_.insert({factory_name(name, type), factory});
}

bool KernelRegistry::has_kernel(const std::string& name, DeviceType type) {
  return factories_.count(factory_name(name, type)) > 0;
}

KernelFactory* KernelRegistry::get_kernel(const std::string& name,
                                          DeviceType type) {
  return factories_.at(factory_name(name, type));
}

std::string KernelRegistry::factory_name(const std::string& name,
                                         DeviceType type) {
  return name + ((type == DeviceType::CPU) ? "_cpu" : "_gpu");
}

KernelRegistry* get_kernel_registry() {
  static KernelRegistry* registry = new KernelRegistry;
  return registry;
}
}
}
