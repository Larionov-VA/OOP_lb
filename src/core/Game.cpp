#include "Game.hpp"


Game::~Game() {
    if (field) {
        delete field;
    }
}


void Game::startNewGame() {
    std::unique_ptr<Entity> player = std::make_unique<Player>();
    GlobalGameConfig::fieldWidth = 10 + rand() % 16;
    GlobalGameConfig::fieldHeight = 10 + rand() % 16;
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
    GlobalGameConfig::fieldWidth = 10 + rand() % 16;
    GlobalGameConfig::fieldHeight = 10 + rand() % 16;
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
    data.gameID = this->gameID;
    if (field) {
        FieldSaveData fieldData = field->getFieldSaveData();
        data.fieldData = fieldData;
    }
    else {
        data.fieldData = FieldSaveData{};
    }
    return data;
}


void Game::saveGame(std::string saveName) {
    SaveData data = collectGameData();
    savesManager.newSave(data, saveName);
}


void Game::setGameData(SaveData& data) {
    this->gameID = data.gameID;
    this->field->setFieldSaveData(data.fieldData);
}


bool Game::loadGame(std::string saveName) {
    try {
        std::unique_ptr<Entity> player = std::make_unique<Player>();
        deleteField();
        SaveData data = savesManager.getLoadGameData(saveName);
        GlobalGameConfig::fieldWidth = data.fieldData.widthField;
        GlobalGameConfig::fieldHeight = data.fieldData.heightField;
        this->field = new GameField(
            std::move(player),
            GlobalGameConfig::fieldWidth,
            GlobalGameConfig::fieldHeight,
            data.fieldData.gameLevel
        );
        setGameData(data);
        return true;
    }
    catch(...) {
        return false;
    }
}


std::vector<std::string> Game::getSavesList(int start, int end) {
    return savesManager.getSavesList(start, end);
}
