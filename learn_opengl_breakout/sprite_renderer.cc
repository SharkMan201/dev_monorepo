//
// Created by abdoe on 23/06/2025.
//
#include "learn_opengl_breakout/sprite_renderer.h"

SpriteRenderer::SpriteRenderer(Shader *shader) : shader(shader) {
  initRenderData();
}

SpriteRenderer::~SpriteRenderer() { glDeleteVertexArrays(1, &quadVao); }

void SpriteRenderer::DrawSprite(Texture2D *texture, glm::vec2 position,
                                glm::vec2 size, float rotate, glm::vec3 color) {
  shader->use();

  glm::mat4 model = glm::mat4(1.0f);
  model = glm::translate(model, glm::vec3(position, 0.0f));

  model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.0f));
  model = glm::rotate(model, glm::radians(rotate), glm::vec3(0.0f, 0.0f, 1.0f));
  model =
      glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.0f));

  model = glm::scale(model, glm::vec3(size, 1.0f));

  shader->setMat4("model", model);
  shader->setVec3("spriteColor", color);

  glActiveTexture(GL_TEXTURE0);
  texture->Bind();

  glBindVertexArray(quadVao);
  glDrawArrays(GL_TRIANGLES, 0, 6);
  glBindVertexArray(0);
}

void SpriteRenderer::initRenderData() {
  // configure vao & vbo
  unsigned int vbo;
  float vertices[] = {0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f,
                      0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
                      1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f};

  glGenVertexArrays(1, &quadVao);
  glGenBuffers(1, &vbo);

  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glBindVertexArray(quadVao);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float),
                        reinterpret_cast<void *>(0));
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
}
