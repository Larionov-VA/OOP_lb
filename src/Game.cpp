#include "Game.hpp"

Game::Game() {
    field = new GameField(20,20);
    gameLevel = 0;
}

Game::~Game() {
}

void Game::start() {
    field->show();
}