load("//third_party/opengl:linkopts.bzl", "OPENGL_LINKOPTS")

cc_binary(
    name = "learn_opengl_advanced",
    srcs = ["main.cc"],
    data = [
        "//learn_opengl_advanced/shaders",
        "//learn_opengl_advanced/textures",
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
