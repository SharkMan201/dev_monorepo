//
// Created by abdoe on 8/06/2025.
//

#ifndef LOCAL_PATHS_H
#define LOCAL_PATHS_H
#include "tools/cpp/runfiles/runfiles.h"
#include "utils/local_paths/executable_path.h"

#include <stdexcept>
using bazel::tools::cpp::runfiles::Runfiles;

class LocalPaths {
public:
  LocalPaths(const LocalPaths &) = delete;
  LocalPaths &operator=(const LocalPaths &) = delete;

  static std::string getLocalPath(const std::string &path) {
    const LocalPaths *local_paths = getInstance();
    return local_paths->runfiles_->Rlocation(path);
  }

private:
  LocalPaths() {
    const std::string &exec_path = getExecutablePath();
    std::string error;
    runfiles_ = std::unique_ptr<Runfiles>(Runfiles::Create(exec_path, &error));
    // Todo better way to error handle
    if (runfiles_ == nullptr) {
      throw std::runtime_error("Failed to create runfiles");
    }
  }

  static LocalPaths *getInstance() {
    if (!local_paths_) {
      local_paths_ = new LocalPaths();
    }

    return local_paths_;
  }

  std::unique_ptr<Runfiles> runfiles_;
  static LocalPaths *local_paths_;
};
LocalPaths *LocalPaths::local_paths_;
#endif // LOCAL_PATHS_H
