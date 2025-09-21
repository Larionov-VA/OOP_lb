#include "Game.hpp"

Game::Game() {
    field = new GameField(25,25);
    gameLevel = 0;
}

Game::~Game() {
}

void Game::start() {
    field->show();
}