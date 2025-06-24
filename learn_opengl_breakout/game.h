//
// Created by abdoe on 23/06/2025.
//

#ifndef GAME_H
#define GAME_H
#include "learn_opengl_breakout/game_level.h"
#include "learn_opengl_breakout/sprite_renderer.h"

enum GameState { GAME_ACTIVE, GAME_MENU, GAME_WIN };

class Game {
public:
  // game state
  GameState State;
  bool Keys[1024];
  unsigned int Width, Height;
  std::unique_ptr<SpriteRenderer> Renderer;
  std::vector<GameLevel> Levels;
  unsigned int Level;
  std::unique_ptr<GameObject> Player;

  // constructor/destructor
  Game(unsigned int width, unsigned int height);
  ~Game();

  // initialize game state (load all shaders/textures/levels)
  void Init();
  // game loop
  void ProcessInput(float dt);
  void Update(float dt);
  void Render();
};

#endif // GAME_H
