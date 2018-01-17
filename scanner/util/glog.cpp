
#include "glog/logging.h"

#include <atomic>

namespace scanner {
namespace {
std::atomic_flag glog_initialized;
}

void init_glog(const char* prog_name) {
  if (!glog_initialized.test_and_set()) {
    google::InitGoogleLogging(prog_name);
  }
}

}
