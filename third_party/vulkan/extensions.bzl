load("//third_party/vulkan:repositories.bzl", "vulkan_repositories")

def _vulkan_repositories_impl(_ctx):
    vulkan_repositories()

vulkan_module = module_extension(
    implementation = _vulkan_repositories_impl,
)