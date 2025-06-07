//
// Created by abdoe on 7/06/2025.
//

#ifndef FILE_READER_H
#define FILE_READER_H

#include "tools/cpp/runfiles/runfiles.h"
#include "utils/files/executable_path.h"

#include <filesystem>
#include <fstream>
#include <iostream>

namespace FilesUtils {
using bazel::tools::cpp::runfiles::Runfiles;
class FileReader {
public:
  FileReader(const FileReader &) = delete;
  FileReader &operator=(const FileReader &) = delete;

  static std::string readFile(const std::string &path) {
    FileReader *reader = getInstance();
    std::ifstream file(reader->runfiles_->Rlocation(path),
                       std::ios::ate | std::ios::binary);

    // todo, better error handling ?
    if (!file.is_open()) {
      throw std::runtime_error("failed to open file");
    }

    const size_t file_size = file.tellg();
    std::vector<char> buffer(file_size);

    file.seekg(0);
    file.read(buffer.data(), file_size);

    file.close();
    return std::string{buffer.begin(), buffer.end()};
  }

private:
  FileReader() {
    const std::string &exec_path = getExecutablePath();
    std::string error;
    runfiles_ = std::unique_ptr<Runfiles>(Runfiles::Create(exec_path, &error));
    // Todo better way to error handle
    if (runfiles_ == nullptr) {
      throw std::runtime_error("Failed to create runfiles");
    }
  }

  static FileReader *getInstance() {
    if (!file_reader_) {
      file_reader_ = new FileReader();
    }

    return file_reader_;
  }

  static FileReader *file_reader_;
  std::unique_ptr<Runfiles> runfiles_;
};

FileReader *FileReader::file_reader_;
} // namespace FilesUtils

#endif // FILE_READER_H
