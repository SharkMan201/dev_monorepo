//
// Created by abdoe on 7/06/2025.
// Disclaimer, this code was generated by ChatGPT, didn't validate it for
// Linux or macOS

#ifndef EXECUTABLE_PATH_H
#define EXECUTABLE_PATH_H
#include <stdexcept>
#include <string>

#if defined(_WIN32)
#include <windows.h>
#elif defined(__APPLE__)
#include <limits.h>
#include <mach-o/dyld.h>
#else
#include <limits.h>
#include <unistd.h>
#endif
inline std::string getExecutablePath() {
#if defined(_WIN32)
  char buffer[MAX_PATH];
  DWORD length = GetModuleFileNameA(nullptr, buffer, MAX_PATH);
  if (length == 0 || length == MAX_PATH) {
    throw std::runtime_error("Failed to get executable path on Windows");
  }
  return {buffer, length};

#elif defined(__APPLE__)
  char buffer[PATH_MAX];
  uint32_t size = sizeof(buffer);
  if (_NSGetExecutablePath(buffer, &size) != 0) {
    throw std::runtime_error("Buffer too small for executable path on macOS");
  }
  return {buffer};

#else // Linux / Unix
  char buffer[PATH_MAX];
  ssize_t count = readlink("/proc/self/exe", buffer, sizeof(buffer));
  if (count == -1 || count == sizeof(buffer)) {
    throw std::runtime_error("Failed to get executable path on Linux");
  }
  return {buffer, count};
#endif
} // namespace LocalPaths
#endif // EXECUTABLE_PATH_H
