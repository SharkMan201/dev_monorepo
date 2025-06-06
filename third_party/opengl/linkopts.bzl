# Common linker options for OpenGL programs.
# Pass to linkopts in cc_binary.
OPENGL_LINKOPTS = select({
    "@platforms//os:linux": [
        "-lGL",
    ],
    "@platforms//os:windows": [
        "-DEFAULTLIB:opengl32.lib",
    ],
    "//conditions:default": [],
})
