//
// Created by abdoe on 24/06/2025.
//

#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H
#include "learn_opengl_breakout/sprite_renderer.h"
#include "learn_opengl_utils/texture.h"

#include <glm/glm.hpp>

class GameObject {
public:
  // object state
  glm::vec2 Position, Size, Velocity;
  glm::vec3 Color;
  float Rotation;
  bool IsSolid;
  bool Destroyed;

  // render state
  Texture2D *Sprite;
  GameObject();
  GameObject(glm::vec2 pos, glm::vec2 size, Texture2D *sprite,
             glm::vec3 color = glm::vec3(1.0f),
             glm::vec2 velocity = glm::vec2(0.0f, 0.0f));

  // draw sprite
  virtual void Draw(SpriteRenderer &renderer) const;
};

#endif // GAME_OBJECT_H
