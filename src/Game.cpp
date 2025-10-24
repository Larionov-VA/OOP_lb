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
    field->show();
}


bool Game::performAnAction(char playerAction) {
    if (!field->playerAlive()) {
        return false;
    }
    field->playerTurn(playerAction);
    field->update();
    field->summonsTurn();
    field->enemyTurn();
    field->buildingsTurn();
    field->update();
    field->show();
    return true;
}


std::shared_ptr<PlayerData> Game::getPlayerData() {
    return field->getPlayerData();
};


std::shared_ptr<std::vector<EnemyData>> Game::getEnemyData() {
    return field->getEnemyData();
};


std::vector<wchar_t> Game::getFieldData() {
    return field->show();
}


void Game::stopGame() {
    delete field;
}


// void Game::startGameLoop() {

// }