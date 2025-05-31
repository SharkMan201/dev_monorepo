# env_setup/BUILD
load("@rules_cc//cc:defs.bzl", "cc_binary")

cc_binary(
    name = "env_setup",
    srcs = glob(["*.cpp"]),
    deps = [
        "//third_party/glfw",
        "//third_party/glm",
        "//third_party/vulkan:vulkan_cc_library"
    ],
)