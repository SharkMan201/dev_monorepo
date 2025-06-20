//
// Created by abdoe on 7/06/2025.
//

#ifndef SHADER_H
#define SHADER_H
#include "third_party/glad/include/glad/glad.h"
#include "utils/files/file_reader.h"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>

class Shader {
public:
  // the program id
  unsigned int id_;

  // constructor to read and build the shaders
  Shader(const char *vertex_path, const char *fragment_path, const char* geometry_path = nullptr) {
    // 1. retrieve the vertex/fragment source code from file_path
    std::string vertex_code = FilesUtils::FileReader::readFile(vertex_path);
    std::string fragment_code = FilesUtils::FileReader::readFile(fragment_path);
    std::string geometry_code;
    if (geometry_path) {
      geometry_code = FilesUtils::FileReader::readFile(geometry_path);
    }

    // 2. compile shaders
    uint32_t vertex = compileShader(GL_VERTEX_SHADER, vertex_code);
    uint32_t fragment = compileShader(GL_FRAGMENT_SHADER, fragment_code);
    uint32_t geometry = -1;
    if (!geometry_code.empty()) {
      geometry = compileShader(GL_GEOMETRY_SHADER, geometry_code);
    }

    // 3. create shader program
    id_ = glCreateProgram();
    glAttachShader(id_, vertex);
    glAttachShader(id_, fragment);
    if (geometry != -1) {
      glAttachShader(id_, geometry);
    }
    glLinkProgram(id_);

    // check for linking errors
    int success;
    char info_log[512];
    glGetProgramiv(id_, GL_LINK_STATUS, &success);
    if (!success) {
      glGetProgramInfoLog(id_, 512, nullptr, info_log);
      throw std::runtime_error(info_log);
    }

    // delete the shaders as they are linked into the program
    glDeleteShader(vertex);
    glDeleteShader(fragment);
  }
  // use/activate the shader
  void use() { glUseProgram(id_); }
  // utility uniform functions
  void setBool(const std::string &name, bool value) const {
    glUniform1i(glGetUniformLocation(id_, name.c_str()),
                static_cast<int>(value));
  }
  void setInt(const std::string &name, int value) const {
    glUniform1i(glGetUniformLocation(id_, name.c_str()), value);
  }
  void setFloat(const std::string &name, float value) const {
    glUniform1f(glGetUniformLocation(id_, name.c_str()), value);
  }

  void setMat4(const std::string &name, const glm::mat4 &value) {
    glUniformMatrix4fv(glGetUniformLocation(id_, name.c_str()), 1, GL_FALSE,
                       glm::value_ptr(value));
  }

  void setVec3(const std::string &name, const float v0, const float v1,
               const float v2) {
    glUniform3f(glGetUniformLocation(id_, name.c_str()), v0, v1, v2);
  }

  void setVec3(const std::string &name, const glm::vec3 &value) {
    setVec3(name, value.x, value.y, value.z);
  }

  void destroy() { glDeleteProgram(id_); }

private:
  uint32_t compileShader(int shader_type, std::string shader_code) {
    uint32_t shader;
    int success;
    char info_log[512];

    // vertex shader
    shader = glCreateShader(shader_type);
    const char *code_c_str = shader_code.c_str();
    glShaderSource(shader, 1, &code_c_str, nullptr);
    glCompileShader(shader);

    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
      glGetShaderInfoLog(shader, 512, nullptr, info_log);
      throw std::runtime_error(info_log);
    }

    return shader;
  }
};

#endif // SHADER_H
