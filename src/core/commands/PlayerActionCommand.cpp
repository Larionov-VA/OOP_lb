#include "PlayerActionCommand.hpp"


void PlayerActionCommand::execute(
    IGameController* gameController,
    GameState& currentState,
    GameState& prevState,
    int* selector,
    int mod,
    std::vector<std::string> loadMenuOptions,
    int* startList,
    int* endList,
    std::string* errMessage,
    bool* firstCall
) {
    (void)prevState;(void)errMessage;(void)firstCall;(void)mod;
    if (currentState == GameState::InGame) {
        if (this->actionSymbol >= StandardKeyboardLayout::KEY1 && this->actionSymbol <= StandardKeyboardLayout::KEY4) {
            gameController->performAnAction((char)('0' + (this->actionSymbol - '1')));
            return;
        }
        bool consumed = false;
        consumed = !gameController->performAnAction(this->actionSymbol);
        if (consumed) {
            currentState = GameState::GameOver;
        }
        return;
    }
    if (currentState == GameState::LoadMenu) {
        if (this->actionSymbol == StandardKeyboardLayout::KEYUP) {
            if (*selector == 0 && *startList > 0) {
                *startList = std::max(0, *startList - 10);
                *endList = *startList + 10;
                loadMenuOptions = gameController->getSavesList(*startList, *endList);
                *selector = (int)loadMenuOptions.size() - 1;
            } else {
                *selector = std::max(0, *selector - 1);
            }
        }
        if (this->actionSymbol == StandardKeyboardLayout::KEYDOWN) {
            if (*selector == (int)loadMenuOptions.size() - 1) {
                int totalSaves = (int)gameController->getSavesList(0,-1).size();
                if (*endList < totalSaves) {
                    *startList = *endList;
                    *endList = std::min(*startList + 10, totalSaves);
                    loadMenuOptions = gameController->getSavesList(*startList, *endList);
                    *selector = 0;
                }
            } else {
                *selector = std::min((int)loadMenuOptions.size() - 1, *selector + 1);
            }
        }
    }
    else {
        if (this->actionSymbol == StandardKeyboardLayout::KEYUP) {
            *selector = (*selector + 1) % mod;
        }
        if (this->actionSymbol == StandardKeyboardLayout::KEYDOWN) {
            *selector = (*selector - 1 + mod) % mod;
        }
    }
}