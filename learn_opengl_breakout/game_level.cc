//
// Created by abdoe on 24/06/2025.
//

#include "learn_opengl_breakout/game_level.h"

#include "learn_opengl_breakout/resource_manager.h"

void GameLevel::Load(const char *file, unsigned int levelWidth,
                     unsigned int levelHeight) {
  // clear old data
  Bricks.clear();
  // load from file
  GameLevel level;
  unsigned int tileCode;
  auto lines = FilesUtils::FileReader::readFileLines(file);
  std::vector<std::vector<unsigned int>> tileData;
  for (const auto &line : lines) {
    std::vector<unsigned int> row;
    std::istringstream sstream(line);
    while (sstream >> tileCode)
      row.push_back(tileCode);
    tileData.push_back(row);
  }

  if (tileData.size() > 0)
    init(tileData, levelWidth, levelHeight);
}

void GameLevel::Draw(SpriteRenderer &renderer) {
  for (const auto &brick : Bricks) {
    if (!brick.Destroyed)
      brick.Draw(renderer);
  }
}

bool GameLevel::IsCompleted() {
  for (const auto &brick : Bricks) {
    if (!brick.IsSolid && !brick.Destroyed)
      return false;
  }
  return true;
}

void GameLevel::init(const std::vector<std::vector<unsigned int>> &tileData,
                     unsigned int levelWidth, unsigned int levelHeight) {
  // calculate dimensions
  unsigned int height = tileData.size();
  unsigned int width = tileData[0].size();
  float unitWidth = levelWidth / static_cast<float>(width);
  float unitHeight = levelHeight / height;

  // initialize level tiles based on tileData
  for (unsigned int y = 0; y < height; y++) {
    for (unsigned int x = 0; x < width; x++) {
      glm::vec2 pos(unitWidth * x, unitHeight * y);
      glm::vec2 size(unitWidth, unitHeight);

      // check block type from level data (2D level array)
      if (tileData[y][x] == 1) { // solid
        GameObject obj(pos, size, ResourceManager::GetTexture("block_solid"),
                       glm::vec3(0.8f, 0.8f, 0.7f));
        obj.IsSolid = true;
        Bricks.push_back(obj);
      } else if (tileData[y][x] > 1) {
        glm::vec3 color = glm::vec3(1.0f); // original: white
        if (tileData[y][x] == 2)
          color = glm::vec3(0.2f, 0.6f, 1.0f);
        else if (tileData[y][x] == 3)
          color = glm::vec3(0.0f, 0.7f, 0.0f);
        else if (tileData[y][x] == 4)
          color = glm::vec3(0.8f, 0.8f, 0.4f);
        else if (tileData[y][x] == 5)
          color = glm::vec3(1.0f, 0.5f, 0.0f);
        Bricks.push_back(
            GameObject(pos, size, ResourceManager::GetTexture("block"), color));
      }
    }
  }
}
