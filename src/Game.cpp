#include "Game.hpp"

Game::Game() {
}


Game::~Game() {
    delete field;
}


void Game::startGame() {
    std::unique_ptr<Entity> player = std::make_unique<Player>();
    field = new GameField(
        std::move(player),
        GlobalGameConfig::fieldWidth,
        GlobalGameConfig::fieldHeight,
        GlobalGameConfig::difficulty
    );
    while (field->playerAlive()) {
        field->show();
        field->playerTurn();
        field->summonsTurn();
        field->enemyTurn();
        field->buildingsTurn();
        field->update();
    }
}


void Game::stopGame() {

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