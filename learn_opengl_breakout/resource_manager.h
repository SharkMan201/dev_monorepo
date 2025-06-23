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

  static Shader *LoadShader(const char *vShaderFile, const char *fShaderFile,
                            const char *gShaderFile, const char *name);
  static Shader *GetShader(const char *name);
  static Texture2D *LoadTexture(const char *file, bool alpha, const char *name);
  static Texture2D *GetTexture(const char *name);
  static void Clear();

private:
  ResourceManager() = default;
  static std::unique_ptr<Shader> loadShaderFromFile(const char *vShaderFile,
                                                    const char *fShaderFile,
                                                    const char *gShaderFile);
  static std::unique_ptr<Texture2D> loadTextureFromFile(const char *file,
                                                        bool alpha);
};

#endif // RESOURCE_MANAGER_H
