//
// Created by abdoe on 23/06/2025.
//
#include "learn_opengl_breakout/game.h"

Game::Game(unsigned int width, unsigned int height)
    : State(GAME_ACTIVE), Keys(), Width(width), Height(height) {}

Game::~Game() {}

void Game::Init() {}

void Game::ProcessInput(float dt) {}

void Game::Update(float dt) {}

void Game::Render() {}
