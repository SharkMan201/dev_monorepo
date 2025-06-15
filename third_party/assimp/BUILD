cc_import(
    name = "assimp_lib_binary",
    interface_library = "assimp-vc143-mtd.lib",
    shared_library = "assimp-vc143-mtd.dll",
)

# TODO: make it cross platform !!
cc_library(
    name = "assimp",
    hdrs = glob([
        "include/**/*.h",
        "include/**/*.hpp",
        "include/**/*.inl",
    ]),
    strip_include_prefix = "include",
    visibility = ["//visibility:public"],
    deps = [
        ":assimp_lib_binary",
    ],
)
