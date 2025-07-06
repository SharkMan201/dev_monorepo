load("@rules_python//python:py_binary.bzl", "py_binary")
load("@rules_python//python:py_library.bzl", "py_library")

py_library(
    name = "local_paths",
    srcs = [
        "local_paths.py",
    ],
    deps = [
        "@pypi//bazel_runfiles",
    ],
)

py_library(
    name = "opencv_manager_lib",
    srcs = [
        "opencv_manager.py",
    ],
    deps = [
        "@pypi//numpy",
        "@pypi//opencv_python",
        ":local_paths",
    ],
)

py_binary(
    name = "logo_automation",
    srcs = [
        "logo_automation.py",
    ],
    data = [
        "//logo_automation/assets",
        "//logo_automation/input",
    ],
    deps = [
        ":opencv_manager_lib",
        ":local_paths",
    ],
)
