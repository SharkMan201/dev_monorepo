//
// Created by abdoe on 23/06/2025.
//
#include "learn_opengl_breakout/game.h"

#include "learn_opengl_breakout/resource_manager.h"

#include <GLFW/glfw3.h>

// Initial size of the player paddle
const glm::vec2 PLAYER_SIZE(100.0f, 20.0f);
// Initial velocity of the player paddle
const float PLAYER_VELOCITY(500.0f);

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
  ResourceManager::LoadTexture(
      "_main/learn_opengl_breakout/textures/background.jpg", false,
      "background");
  ResourceManager::LoadTexture("_main/learn_opengl_breakout/textures/block.png",
                               false, "block");
  ResourceManager::LoadTexture(
      "_main/learn_opengl_breakout/textures/block_solid.png", false,
      "block_solid");
  ResourceManager::LoadTexture(
      "_main/learn_opengl_breakout/textures/paddle.png", true, "paddle");

  // load levels
  GameLevel one, two, three, four;
  one.Load("_main/learn_opengl_breakout/levels/one.lvl", Width, Height / 2);
  two.Load("_main/learn_opengl_breakout/levels/two.lvl", Width, Height / 2);
  three.Load("_main/learn_opengl_breakout/levels/three.lvl", Width, Height / 2);
  four.Load("_main/learn_opengl_breakout/levels/four.lvl", Width, Height / 2);
  Levels.push_back(one);
  Levels.push_back(two);
  Levels.push_back(three);
  Levels.push_back(four);
  Level = 0;

  // configure player
  glm::vec2 playerPos =
      glm::vec2(Width / 2.0f - PLAYER_SIZE.x / 2.0f, Height - PLAYER_SIZE.y);
  Player = std::make_unique<GameObject>(playerPos, PLAYER_SIZE,
                                        ResourceManager::GetTexture("paddle"));
}

void Game::ProcessInput(float dt) {
  if (State == GAME_ACTIVE) {
    float velocity = PLAYER_VELOCITY * dt;
    // move playerboard
    if (Keys[GLFW_KEY_A]) {
      if (Player->Position.x >= 0.0f)
        Player->Position.x -= velocity;
    }
    if (Keys[GLFW_KEY_D]) {
      if (Player->Position.x <= Width - Player->Size.x)
        Player->Position.x += velocity;
    }
  }
}

void Game::Update(float dt) {}

void Game::Render() {
  if (State == GAME_ACTIVE) {
    // draw background
    Renderer->DrawSprite(ResourceManager::GetTexture("background"),
                         glm::vec2(0.0f, 0.0f), glm::vec2(Width, Height), 0.0f);
    // draw level
    Levels[Level].Draw(*Renderer);

    // draw player
    Player->Draw(*Renderer);
  }
}
