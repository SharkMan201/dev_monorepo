//
// Created by abdoe on 23/06/2025.
//

#ifndef SPRITE_RENDERER_H
#define SPRITE_RENDERER_H
#include "learn_opengl_utils/shader.h"
#include "learn_opengl_utils/texture.h"

class SpriteRenderer {
public:
  explicit SpriteRenderer(Shader *shader);
  ~SpriteRenderer();

  void DrawSprite(Texture2D *texture, glm::vec2 position,
                  glm::vec2 size = glm::vec2(10.0f, 10.0f), float rotate = 0.0f,
                  glm::vec3 color = glm::vec3(1.0));

private:
  Shader *shader;
  unsigned int quadVao;

  void initRenderData();
};

#endif // SPRITE_RENDERER_H
