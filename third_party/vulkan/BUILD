load("@rules_cc//cc:defs.bzl", "cc_library")

config_setting(
    name = "android",
    values = {
        # same as that of mediapipe
        "crosstool_top": "//external:android/crosstool",
    },
)

config_setting(
    name = "windows",
    constraint_values = ["@platforms//os:windows"],
)

config_setting(
    name = "darwin",
    constraint_values = ["@platforms//os:macos"],
)

VULKAN_LINKOPTS = select({
    ":android": [],
    ":windows": [],
    ":darwin": [],
    "//conditions:default": ["-lvulkan"],
})

VULKAN_LIBRARIES = select(
    {
        ":windows": ["@vulkan_windows//:vulkan_cc_library"],
        ":darwin": ["@vulkan_macos//:vulkan_cc_library"],
        ":android": ["@vulkan_android//:vulkan_cc_library"],
        "//conditions:default": ["@vulkan_linux//:vulkan_cc_library"],
    },
    no_match_error = "No configuration matches the supported platforms",
)

cc_library(
    name = "vulkan_cc_library",
    linkopts = VULKAN_LINKOPTS,
    visibility = ["//visibility:public"],
    deps = VULKAN_LIBRARIES,
)
