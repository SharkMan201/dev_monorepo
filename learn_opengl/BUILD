load("@rules_cc//cc:defs.bzl", "cc_binary")
load("@rules_pkg//pkg:zip.bzl", "pkg_zip")
load("//third_party/opengl:linkopts.bzl", "OPENGL_LINKOPTS")

cc_binary(
    name = "learn_opengl",
    srcs = [
        "camera.h",
        "main.cc",
        "mesh.h",
    ],
    data = [
        "//learn_opengl/textures",
    ],
    linkopts = OPENGL_LINKOPTS,
    deps = [
        "//learn_opengl/shaders",
        "//third_party/glad",
        "//third_party/glfw",
        "//third_party/stb_image",
        "@assimp",
        "@glm",
    ],
)

pkg_zip(
    name = "learn_opengl_pkg",
    srcs = [":learn_opengl"],
    include_runfiles = True,
)
