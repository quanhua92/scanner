
#pragma once

#include "scanner/engine/metadata.h"

#include <map>
#include <mutex>

namespace scanner {
namespace internal {

class TableMetaCache {
 public:
  TableMetaCache(storehouse::StorageBackend* storage,
                 const DatabaseMetadata& meta);

  const TableMetadata& at(const std::string& table_name) const;

  const TableMetadata& at(i32 table_id) const;

  bool exists(const std::string& table_name) const;

  bool exists(i32 table_id) const;

  bool has(const std::string& table_name) const;

  void update(const TableMetadata& meta);

  void prefetch(const std::vector<std::string>& table_names);

  void write_megafile();

 private:
  void memoized_read(const std::string& table_name) const;

  void memoized_read(i32 table_id) const;

  storehouse::StorageBackend* storage_;
  const DatabaseMetadata& meta_;
  mutable std::mutex lock_;
  mutable std::unordered_map<i32, TableMetadata> cache_;
};

}
}
