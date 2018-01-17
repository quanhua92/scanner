
#pragma once

#include "scanner/api/database.h"
#include "scanner/util/common.h"

#include "storehouse/storage_backend.h"
#include "storehouse/storage_config.h"

#include <string>

namespace scanner {
namespace internal {

Result ingest_videos(storehouse::StorageConfig* storage_config,
                     const std::string& db_path,
                     const std::vector<std::string>& table_names,
                     const std::vector<std::string>& paths,
                     bool inplace,
                     std::vector<FailedVideo>& failed_videos);

// void ingest_images(storehouse::StorageConfig *storage_config,
//                    const std::string &db_path, const std::string &table_name,
//                    const std::vector<std::string> &paths);
}
}
