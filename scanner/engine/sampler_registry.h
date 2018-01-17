
#pragma once

#include "scanner/api/op.h"
#include "scanner/engine/op_info.h"

#include "scanner/util/common.h"

#include <map>

namespace scanner {
namespace internal {

class SamplerRegistry {
 public:
  void add_sampler(const std::string& name, Sampler* info);

  OpInfo* get_op_info(const std::string& name) const;

  bool has_op(const std::string& name) const;

 private:
  std::map<std::string, OpInfo*> ops_;
};

OpRegistry* get_op_registry();
}
}
