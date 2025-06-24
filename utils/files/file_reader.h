//
// Created by abdoe on 7/06/2025.
//

#ifndef FILE_READER_H
#define FILE_READER_H

#include "utils/local_paths/local_paths.h"

#include <boost/algorithm/string.hpp>
#include <filesystem>
#include <fstream>
#include <iostream>

namespace FilesUtils {
class FileReader {
public:
  FileReader(const FileReader &) = delete;
  FileReader &operator=(const FileReader &) = delete;

  static std::string readFile(const std::string &path) {
    FileReader *reader = getInstance();
    auto local_path = LocalPaths::getLocalPath(path);
    std::ifstream file(local_path, std::ios::ate | std::ios::binary);

    // todo, better error handling ?
    if (!file.is_open()) {
      throw std::runtime_error("failed to open file at path: " + path +
                               " generated local path is: " + local_path);
    }

    const size_t file_size = file.tellg();
    std::vector<char> buffer(file_size);

    file.seekg(0);
    file.read(buffer.data(), file_size);

    file.close();
    return std::string{buffer.begin(), buffer.end()};
  }

  static std::vector<std::string> readFileLines(const std::string &path) {
    auto fileContent = readFile(path);
    std::vector<std::string> ret;
    boost::split(ret, fileContent, boost::is_any_of("\n"));

    return ret;
  }

private:
  FileReader() = default;

  static FileReader *getInstance() {
    if (!file_reader_) {
      file_reader_ = new FileReader();
    }

    return file_reader_;
  }

  static FileReader *file_reader_;
};
} // namespace FilesUtils

#endif // FILE_READER_H
