load("//third_party/opengl:linkopts.bzl", "OPENGL_LINKOPTS")

cc_library(
    name = "learn_opengl_breakout_lib",
    srcs = [
        "game.cc",
        "resource_manager.cc",
    ],
    hdrs = [
        "game.h",
        "resource_manager.h",
    ],
    linkopts = OPENGL_LINKOPTS,
    deps = [
        "//learn_opengl_utils",
        "//third_party/glad",
        "//third_party/glfw",
        "//third_party/stb_image",
        "//utils/local_paths",
        "@glm",
    ],
)

cc_binary(
    name = "learn_opengl_breakout",
    srcs = ["main.cc"],
    deps = ["learn_opengl_breakout_lib"],
)
