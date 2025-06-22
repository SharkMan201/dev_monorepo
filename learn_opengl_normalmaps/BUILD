load("//third_party/opengl:linkopts.bzl", "OPENGL_LINKOPTS")

cc_binary(
    name = "learn_opengl_normalmaps",
    srcs = ["main.cc"],
    data = [
        "//learn_opengl_normalmaps/models",
        "//learn_opengl_normalmaps/shaders",
        "//learn_opengl_normalmaps/textures",
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
