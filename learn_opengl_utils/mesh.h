//
// Created by abdoe on 15/06/2025.
//

#ifndef MESH_H
#define MESH_H
#include "learn_opengl_utils/shader.h"

#include <glm/glm.hpp>
#include <string>
#include <utility>
#include <vector>

using namespace std;

struct Vertex {
  glm::vec3 position;
  glm::vec3 normal;
  glm::vec2 tex_coords;
};

struct Texture {
  uint32_t id;
  string type;
  string path;
};

class Mesh {
public:
  Mesh &operator=(const Mesh &) = delete;
  Mesh(const Mesh &) = delete;

  Mesh(vector<Vertex> &vertices, vector<uint32_t> &indices,
       vector<Texture> &textures) {
    vertices_ = std::move(vertices);
    indices_ = std::move(indices);
    textures_ = std::move(textures);

    SetupMesh();
  }

  void Draw(Shader &shader) {
    // WARNING: SHADER NEEDS TO BE IN USE BEFORE THIS GETS CALLED, oversight ?
    // diffuse & specular texture idx
    uint32_t diffuse_nr = 1;
    uint32_t specular_nr = 1;
    for (auto i = 0; i < textures_.size(); i++) {
      // activate the texture
      glActiveTexture(GL_TEXTURE0 + i);
      string number;
      const string &name = textures_[i].type;
      if (name == "texture_diffuse")
        number = to_string(diffuse_nr++);
      else if (name == "texture_specular")
        number = to_string(specular_nr++);

      shader.setInt("material." + name + number, i);
      glBindTexture(GL_TEXTURE_2D, textures_[i].id);
    }

    shader.setFloat("material.shininess", 32.0f);

    glActiveTexture(GL_TEXTURE0);

    // draw mesh
    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, indices_.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
  }

private:
  void SetupMesh() {
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);

    glBindVertexArray(vao);
    // setup vbo
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices_.size() * sizeof(Vertex),
                 vertices_.data(), GL_STATIC_DRAW);
    // setup ebo
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_.size() * sizeof(uint32_t),
                 indices_.data(), GL_STATIC_DRAW);

    // setup shader layout params
    // verterx positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          reinterpret_cast<void *>(offsetof(Vertex, position)));
    // normals
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          reinterpret_cast<void *>(offsetof(Vertex, normal)));
    // texture coords
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(
        2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
        reinterpret_cast<void *>(offsetof(Vertex, tex_coords)));

    // unbind vao
    glBindVertexArray(0);
  }

public:
  // render data
  uint32_t vao, vbo, ebo;
  // mesh data
  vector<Vertex> vertices_;
  vector<uint32_t> indices_;
  vector<Texture> textures_;
};

#endif // MESH_H
