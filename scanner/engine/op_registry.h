
#pragma once

#include "scanner/api/op.h"
#include "scanner/engine/op_info.h"

#include "scanner/util/common.h"

#include <map>

namespace scanner {
namespace internal {

class OpRegistry {
 public:
  Result add_op(const std::string& name, OpInfo* info);

  OpInfo* get_op_info(const std::string& name) const;

  bool has_op(const std::string& name) const;

 private:
  std::map<std::string, OpInfo*> ops_;
};

OpRegistry* get_op_registry();
}
}
