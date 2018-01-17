
#pragma once

#include <glog/logging.h>
#include <libgen.h>
#include <stdio.h>
#include <sys/stat.h>
#include <cstring>
#include <sstream>
#include <string>
#include <vector>

namespace scanner {

///////////////////////////////////////////////////////////////////////////////
/// Path utils
inline std::string dirname_s(const std::string& path) {
  char* path_copy = strdup(path.c_str());
  char* dir = dirname(path_copy);
  return std::string(dir);
}

inline std::string basename_s(const std::string& path) {
  char* path_copy = strdup(path.c_str());
  char* base = basename(path_copy);
  return std::string(base);
}

int mkdir_p(const char* path, mode_t mode);

void temp_file(FILE** file, std::string& name);

void temp_file(std::string& name);

void temp_dir(std::string& name);

void download(const std::string& url, const std::string& local_path);

std::string download_temp(const std::string& url);

void delete_file(const std::string& path);

std::vector<uint8_t> read_entire_file(const std::string& file_name);
}
