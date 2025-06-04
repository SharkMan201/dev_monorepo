# hello_triangle/BUILD
load("@rules_cc//cc:defs.bzl", "cc_binary")

cc_binary(
    name = "hello_triangle",
    srcs = [
        "main.cc",
    ],
    data = [
        "//hello_triangle/shaders:frag_shader",
        "//hello_triangle/shaders:vert_shader",
    ],
    deps = [
        "//third_party/glfw",
        "//third_party/vulkan:vulkan_cc_library",
        "@bazel_tools//tools/cpp/runfiles",
        "@glm",
    ],
)
