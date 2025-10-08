#include "Game.hpp"


Game::Game(int width, int height, unsigned level) {
    std::unique_ptr<Entity> player = std::make_unique<Player>();
    field = new GameField(std::move(player), width, height, level);
    gameLevel = level;
}


Game::Game() : Game(25, 25, 1){
}


Game::~Game() {
    delete field;
}


void Game::startGameLoop() {
    while (field->playerAlive()) {
        field->show();
        field->playerTurn();
        field->summonsTurn();
        field->enemyTurn();
        field->buildingsTurn();
        field->update();
    }
    field->show();
    std::cout << "YOU DIED\n";
}