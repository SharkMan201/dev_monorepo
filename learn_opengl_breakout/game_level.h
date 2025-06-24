//
// Created by abdoe on 24/06/2025.
//

#ifndef GAME_LEVEL_H
#define GAME_LEVEL_H
#include "learn_opengl_breakout/game_object.h"

#include <vector>

class GameLevel {
public:
  // level state
  std::vector<GameObject> Bricks;
  // constructor
  GameLevel() {}
  // loads level from file
  void Load(const char *file, unsigned int levelWidth,
            unsigned int levelHeight);
  // render level
  void Draw(SpriteRenderer &renderer);
  // check if the level is completed
  bool IsCompleted();

private:
  // initialize level from tile data
  void init(const std::vector<std::vector<unsigned int>> &tileData,
            unsigned int levelWidth, unsigned int levelHeight);
};

#endif // GAME_LEVEL_H
