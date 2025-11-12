#include "Game.hpp"


Game::Game() {
    head = new SavesTreeNode{this};
}


Game::~Game() {
    if (field) {
        delete field;
    }
    // if (head) {
    //     delete head;
    // }
}


void Game::ContinueGame(int gameID) {
    this->gameID = gameID;

}


void Game::startNewGame() {
    std::unique_ptr<Entity> player = std::make_unique<Player>();
    // this->head = (SavesTreeNode*)this;
    this->field = new GameField(
        std::move(player),
        GlobalGameConfig::fieldWidth,
        GlobalGameConfig::fieldHeight,
        GlobalGameConfig::gameLevel
    );
    this->gameID = (int)std::time(nullptr);
    if (head) {
        head->addChild(this->field);
    }

    // this->addChild((ISaveManager*)this->field);
    // this->getChilds()[0]->addChild((ISaveManager*)this->field);
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
    head->addChild(this->field);
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
        if (head) {
            head->clearChildren();
        }
        delete this->field;
        this->field = nullptr;
    }
}


void Game::saveGame() {
    SaveManager saver;
    saver.newSave(gameID);
    head->saveState(gameID);
    // std::string fullPathForSave = SAVES_PATH + std::to_string(gameID) + CUR_SAVES_DIR + "Game.txt";
    // FileHandler flog{"gameLog.txt", std::ios::app};
    // flog.write(fullPathForSave);
}


void Game::loadGame() {

}


int Game::getGameID() {
    return this->gameID;
}


void Game::setGameID(int newGameID) {
    this->gameID = newGameID;
}


void Game::saveState(int saveID) {
    std::string fullPathForSave = SAVES_PATH + std::to_string(saveID) + GAME_SAVES_DIR + "data.txt";
    FileHandler file{fullPathForSave, std::ios::out};
    file.write(std::to_string(this->getGameID()));
}


void Game::loadState(int loadID) {
    (void)loadID;
}