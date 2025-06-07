cc_test(
    name = "files_tests",
    srcs = ["file_reader_unittest.cc"],
    data = [
        "dummy_file.txt",
    ],
    target_compatible_with = [
        "@platforms//os:windows",
        "@platforms//cpu:x86_64",
    ],
    deps = [
        "//utils/files",
        "@gtest//:gtest_main",
    ],
)
