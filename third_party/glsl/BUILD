"""
"""

load("//third_party/glsl:toolchain.bzl", "glsl_toolchain")

toolchain_type(
    name = "toolchain_type",
)

glsl_toolchain(
    name = "glsl_tc",
    glslc = select({
        "@platforms//os:linux": "@vulkan_linux//:glslc",
        "@platforms//os:windows": "@vulkan_windows//:glslc",
        "@platforms//os:macos": "@vulkan_macos//:glslc",
    }),
)

toolchain(
    name = "glsl_all_platforms_toolchain",
    toolchain = ":glsl_tc",
    toolchain_type = "//third_party/glsl:toolchain_type",
)
