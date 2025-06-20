load("//third_party/opengl:linkopts.bzl", "OPENGL_LINKOPTS")

cc_binary(
    name = "learn_opengl_instanced_rendering",
    srcs = ["main.cc"],
    data = [
        "//learn_opengl_instanced_rendering/models",
        "//learn_opengl_instanced_rendering/shaders",
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
