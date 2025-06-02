# hello_triangle/BUILD
load("@rules_cc//cc:defs.bzl", "cc_binary")

cc_binary(
    name = "hello_triangle",
    srcs = [
        "main.cc",
    ],
    deps = [
        "//third_party/glfw",
        "//third_party/vulkan:vulkan_cc_library",
        "@glm",
    ],
)
