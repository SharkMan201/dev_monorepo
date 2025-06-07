//
// Created by abdoe on 7/06/2025.
//

#ifndef SHADER_H
#define SHADER_H
#include "third_party/glad/include/glad/glad.h"
#include "utils/files/file_reader.h"

#include <string>

class Shader {
public:
  // the program id
  unsigned int id_;

  // constructor to read and build the shaders
  Shader(const char *vertex_path, const char *fragment_path) {
    // 1. retrieve the vertex/fragment source code from file_path
    const char *vertex_code =
        FilesUtils::FileReader::readFile(vertex_path).c_str();
    const char *fragment_code =
        FilesUtils::FileReader::readFile(fragment_path).c_str();

    // 2. compile shaders
    uint32_t vertex = compileShader(GL_VERTEX_SHADER, vertex_code);
    uint32_t fragment = compileShader(GL_FRAGMENT_SHADER, fragment_code);

    // 3. create shader program
    id_ = glCreateProgram();
    glAttachShader(id_, vertex);
    glAttachShader(id_, fragment);
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

  void destroy() { glDeleteProgram(id_); }

private:
  uint32_t compileShader(int shader_type, const char *shader_code) {
    uint32_t shader;
    int success;
    char info_log[512];

    // vertex shader
    shader = glCreateShader(shader_type);
    glShaderSource(shader, 1, &shader_code, nullptr);
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
