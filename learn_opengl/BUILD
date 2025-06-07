load("//third_party/opengl:linkopts.bzl", "OPENGL_LINKOPTS")

cc_binary(
    name = "learn_opengl",
    srcs = ["main.cc"],
    linkopts = OPENGL_LINKOPTS,
    deps = [
        "//learn_opengl/shaders",
        "//third_party/glad",
        "//third_party/glfw",
    ],
)
