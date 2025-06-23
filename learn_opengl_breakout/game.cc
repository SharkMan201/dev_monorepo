//
// Created by abdoe on 23/06/2025.
//
#include "learn_opengl_breakout/game.h"

#include "learn_opengl_breakout/resource_manager.h"

Game::Game(unsigned int width, unsigned int height)
    : State(GAME_ACTIVE), Keys(), Width(width), Height(height) {}

Game::~Game() {}

void Game::Init() {
  // load shaders
  ResourceManager::LoadShader("_main/learn_opengl_breakout/shaders/shader.vert",
                              "_main/learn_opengl_breakout/shaders/shader.frag",
                              nullptr, "sprite");

  // configure shaders
  glm::mat4 projection =
      glm::ortho(0.0f, static_cast<float>(Width), static_cast<float>(Height),
                 0.0f, -1.0f, 1.0f);
  auto spriteShader = ResourceManager::GetShader("sprite");
  spriteShader->use();
  // the id of the active texture GL_TEXTURE0
  spriteShader->setInt("image", 0);
  spriteShader->setMat4("projection", projection);

  // set renter specific controls
  Renderer = std::make_unique<SpriteRenderer>(spriteShader);

  // load textures
  ResourceManager::LoadTexture(
      "_main/learn_opengl_breakout/textures/awesomeface.png", true, "face");
}

void Game::ProcessInput(float dt) {}

void Game::Update(float dt) {}

void Game::Render() {
  Renderer->DrawSprite(ResourceManager::GetTexture("face"),
                       glm::vec2(200.0f, 200.0f), glm::vec2(300.0f, 400.0f),
                       45.0f, glm::vec3(0.0f, 1.0f, 0.0f));
}
