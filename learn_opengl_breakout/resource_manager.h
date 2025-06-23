//
// Created by abdoe on 23/06/2025.
//

#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H
#include "learn_opengl_utils/shader.h"
#include "learn_opengl_utils/texture.h"

#include <map>
#include <string>

class ResourceManager {
public:
  // resource storage
  static std::map<std::string, std::unique_ptr<Shader>> Shaders;
  static std::map<std::string, std::unique_ptr<Texture2D>> Textures;

  static Shader *LoadShader(const std::string &vShaderFile,
                            const std::string &fShaderFile,
                            const std::string &gShaderFile,
                            const std::string &name);
  static Shader *GetShader(const std::string &name);
  static Texture2D *LoadTexture(const std::string &file, bool alpha,
                                const std::string &name);
  static Texture2D *GetTexture(const std::string &name);
  static void Clear();

private:
  ResourceManager() = default;
  static std::unique_ptr<Shader>
  loadShaderFromFile(const std::string &vShaderFile,
                     const std::string &fShaderFile,
                     const std::string &gShaderFile);
  static std::unique_ptr<Texture2D> loadTextureFromFile(const std::string &file,
                                                        bool alpha);
};

#endif // RESOURCE_MANAGER_H
