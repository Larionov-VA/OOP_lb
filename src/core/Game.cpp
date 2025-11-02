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
        GlobalGameConfig::gameLevel
    );
}


void Game::nextLevel() {
    std::unique_ptr<Entity> player = field->returnPlayer();
    GlobalGameConfig::gameLevel++;
    this->stopGame();
    field = new GameField(
        std::move(player),
        GlobalGameConfig::fieldWidth,
        GlobalGameConfig::fieldHeight,
        GlobalGameConfig::gameLevel
    );
}


void Game::playerLevelUp(char attribute) {
    field->playerLevelUp(attribute);
}


bool Game::performAnAction(char playerAction) {
    if (!field) {
        return false;
    }
    if (!field->playerAlive()) {
        return false;
    }
    bool nextTurnAction = true;
    nextTurnAction = field->playerTurn(playerAction);
    if (!field->getCountOfEnemy()) {
        this->nextLevel();
    }
    if (nextTurnAction) {
        field->update();
        field->enemyTurn();
        field->update();
        field->buildingsTurn();
        field->update();
    }
    return true;
}


std::shared_ptr<PlayerData> Game::getPlayerData() {
    return field->getPlayerData();
};


std::vector<EnemyData> Game::getEnemyData() {
    return field->getEnemyData();
};


std::vector<wchar_t> Game::getFieldData() {
    return field->show();
}


void Game::stopGame() {
    delete field;
    field = nullptr;
}