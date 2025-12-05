#include "Game.hpp"


Game::~Game() {
    if (field) {
        delete field;
    }
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
    player->regenerateLife(1.0);
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
    if (this->field) {
        delete this->field;
        this->field = nullptr;
    }
}


int Game::getGameID() {
    return this->gameID;
}


void Game::setGameID(int newGameID) {
    this->gameID = newGameID;
}

SaveData Game::collectGameData() {
    SaveData data;
    FieldSaveData fieldData = field->getFieldSaveData();
    data.fieldData = fieldData;
    data.gameID = this->gameID;
    return data;
}

void Game::saveGame(std::string saveName) {
    SaveData data = collectGameData();
    savesManager.newSave(data, saveName);
}

bool Game::loadGame() {
    std::vector<std::string> saveList = savesManager.getSavesList();
    for (auto& save : saveList) {
        std::cout << save << '\n';
    }
    return false;
}

std::vector<std::string> Game::getSavesList() {
    return savesManager.getSavesList();
}