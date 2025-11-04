#include "Game.hpp"


Game::~Game() {
    if (field) {
        delete field;
    }
}


void Game::ContinueGame(int gameID) {
    this->gameID = gameID;

}


void Game::startNewGame() {
    std::unique_ptr<Entity> player = std::make_unique<Player>();
    this->field = new GameField(
        std::move(player),
        GlobalGameConfig::fieldWidth,
        GlobalGameConfig::fieldHeight,
        GlobalGameConfig::gameLevel
    );
    this->gameID = (int)std::time(nullptr);
}


void Game::nextLevel() {
    std::unique_ptr<Entity> player = field->returnPlayer();
    GlobalGameConfig::gameLevel++;
    deleteField();
    this->field = new GameField(
        std::move(player),
        GlobalGameConfig::fieldWidth,
        GlobalGameConfig::fieldHeight,
        GlobalGameConfig::gameLevel
    );
}


void Game::playerLevelUp(char attribute) {
    this->field->playerLevelUp(attribute);
}


bool Game::performAnAction(char playerAction) {
    if (!this->field->playerAlive()) {
        GlobalGameConfig::gameLevel = 1;
        return false;
    }
    bool nextTurnAction = true;
    nextTurnAction = field->playerTurn(playerAction);
    if (!field->getCountOfEnemy()) {
        this->nextLevel();
    }
    if (nextTurnAction) {
        this->field->update();
        this->field->enemyTurn();
        this->field->update();
        this->field->buildingsTurn();
        this->field->update();
    }
    return true;
}


std::shared_ptr<PlayerData> Game::getPlayerData() {
    return this->field->getPlayerData();
};


std::vector<EnemyData> Game::getEnemyData() {
    return this->field->getEnemyData();
};


std::vector<wchar_t> Game::getFieldData() {
    return this->field->show();
}


void Game::stopGame() {
    GlobalGameConfig::gameLevel = 1;
    deleteField();
}


void Game::deleteField() {
    delete this->field;
    this->field = nullptr;
}


void Game::saveGame() {
    SaveManager saver;
    saver.newSave(gameID);
}


void Game::loadGame() {

}