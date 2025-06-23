//
// Created by abdoe on 23/06/2025.
//
#include "learn_opengl_breakout/resource_manager.h"
#include "third_party/stb_image/stb_image.h"
#include "utils/local_paths/local_paths.h"

// instantiate static variables
std::map<std::string, std::unique_ptr<Texture2D>> ResourceManager::Textures;
std::map<std::string, std::unique_ptr<Shader>> ResourceManager::Shaders;

Shader *ResourceManager::LoadShader(const char *vShaderFile,
                                    const char *fShaderFile,
                                    const char *gShaderFile, const char *name) {
  Shaders[name] = loadShaderFromFile(vShaderFile, fShaderFile, gShaderFile);
  return Shaders[name].get();
}

Shader *ResourceManager::GetShader(const char *name) {
  return Shaders[name].get();
}

Texture2D *ResourceManager::LoadTexture(const char *file, bool alpha,
                                        const char *name) {
  Textures[name] = loadTextureFromFile(file, alpha);
  return Textures[name].get();
}

Texture2D *ResourceManager::GetTexture(const char *name) {
  return Textures[name].get();
}

void ResourceManager::Clear() {
  for (auto &iter : Shaders) {
    glDeleteProgram(iter.second->id_);
  }
  for (auto &iter : Textures) {
    glDeleteTextures(1, &iter.second->Id);
  }
}

std::unique_ptr<Shader> ResourceManager::loadShaderFromFile(
    const char *vShaderFile, const char *fShaderFile, const char *gShaderFile) {
  Shader shader(vShaderFile, fShaderFile, gShaderFile);
  return std::make_unique<Shader>(shader);
}

std::unique_ptr<Texture2D>
ResourceManager::loadTextureFromFile(const char *file, bool alpha) {
  Texture2D texture;
  if (alpha) {
    texture.InternalFormat = GL_RGBA;
    texture.ImageFormat = GL_RGBA;
  }

  int width, height, nrChannels;
  const auto localPath = LocalPaths::getLocalPath(file);
  unsigned char *data =
      stbi_load(localPath.c_str(), &width, &height, &nrChannels, 0);
  texture.Generate(width, height, data);

  stbi_image_free(data);
  return std::make_unique<Texture2D>(texture);
}
