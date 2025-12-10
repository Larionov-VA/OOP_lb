#include "ControlCommand.hpp"


void ControlCommand::mainMenuExecute(
    IGameController* gameController,
    GameState& currentState,
    GameState& prevState,
    int* selector
) {
    if (this->controlCommand == StandardKeyboardLayout::KEYENTER) {
        if (*selector == 0) {
            if (gameController) gameController->startNewGame();
            currentState = GameState::InGame;
        } else if (*selector == 1) {
            if (gameController) gameController->startNewGame();
            currentState = GameState::LoadMenu;
            prevState = GameState::MainMenu;
        } else if (*selector == 2) {
            currentState = GameState::AutorsMenu;
        } else if (*selector == 3) {
            currentState = GameState::Exit;
        }
    }
    if (this->controlCommand == StandardKeyboardLayout::KEYESC) {
        currentState = GameState::Exit;
    }
}


void ControlCommand::loadMenuExecute(
    IGameController* gameController,
    GameState& currentState,
    GameState& prevState,
    int* selector,
    std::vector<std::string> loadMenuOptions,
    int* startList,
    int* endList,
    std::string* errMessage,
    bool* firstCall
) {
    if (this->controlCommand == StandardKeyboardLayout::KEYENTER) {
        if (gameController && !loadMenuOptions.empty()) {
            std::string selectedSave = loadMenuOptions[*selector];
            try {
                if (gameController->loadGame(selectedSave)) {
                    *startList = 0;
                    *endList = 10;
                    *selector = 0;
                    *firstCall = true;
                    currentState = GameState::InGame;
                    *errMessage = "";
                }
                else {
                    *errMessage = "failed to load save";
                }
            }
            catch(const std::exception& e) {
                *errMessage = e.what();
            }
        }
    }
    if (this->controlCommand == StandardKeyboardLayout::KEYESC) {
        *startList = 0;
        *endList = 10;
        *selector = 0;
        *firstCall = true;
        currentState = prevState;
    }
}


void ControlCommand::inGameExecute(
    GameState& currentState
) {
    if (this->controlCommand == StandardKeyboardLayout::KEYESC) {
        currentState = GameState::PauseMenu;
    }
}


void ControlCommand::levelUpMenuExecute(
    IGameController* gameController,
    GameState& currentState,
    int* selector
) {
    if (this->controlCommand == StandardKeyboardLayout::KEYENTER) {
        if (gameController) {
            char choice = '1' + *selector;
            gameController->playerLevelUp(choice);
        }
        currentState = GameState::InGame;
        *selector = 0;
    }
}


void ControlCommand::gameOverExecute(
    IGameController* gameController,
    GameState& currentState,
    GameState& prevState,
    int* selector
) {
    if (this->controlCommand == StandardKeyboardLayout::KEYENTER) {
        if (*selector == 0) {
            gameController->startNewGame();
            currentState = GameState::InGame;
        }
        else if (*selector == 1) {
            currentState = GameState::LoadMenu;
            prevState = GameState::GameOver;
        }
        else if (*selector == 2) {
            currentState = GameState::MainMenu;
        }
    }
}


void ControlCommand::pauseMenuExecute(
    IGameController* gameController,
    GameState& currentState,
    GameState& prevState,
    int* selector
) {
    if (this->controlCommand == StandardKeyboardLayout::KEYENTER) {
        if (gameController) {
            if (*selector == 0) {
                currentState = GameState::InGame;
            }
            else if (*selector == 1) {
                currentState = GameState::InGame;
                gameController->saveGame();
            }
            else if (*selector == 2) {
                currentState = GameState::LoadMenu;
                prevState = GameState::PauseMenu;
            }
            else if (*selector == 3) {
                currentState = GameState::MainMenu;
            }
        }
        *selector = 0;
    }
    if (this->controlCommand == StandardKeyboardLayout::KEYESC) {
        currentState = GameState::InGame;
        *selector = 0;
    }
}


void ControlCommand::autorsMenuExecute(
    GameState& currentState
) {
    currentState = GameState::MainMenu;
}


void ControlCommand::execute(
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
    (void)mod;
    if (gameController && selector) {
        switch (currentState) {
        case GameState::MainMenu:
            mainMenuExecute(
                gameController,
                currentState,
                prevState,
                selector
            );
            break;
        case GameState::LoadMenu:
            loadMenuExecute(
                gameController,
                currentState,
                prevState,
                selector,
                loadMenuOptions,
                startList,
                endList,
                errMessage,
                firstCall
            );
            break;
        case GameState::InGame:
            inGameExecute(currentState);
            break;
        case GameState::LevelUpMenu:
            levelUpMenuExecute(
                gameController,
                currentState,
                selector
            );
            break;
        case GameState::GameOver:
            gameOverExecute(
                gameController,
                currentState,
                prevState,
                selector
            );
            break;
        case GameState::PauseMenu:
            pauseMenuExecute(
                gameController,
                currentState,
                prevState,
                selector
            );
            break;
        case GameState::AutorsMenu:
            autorsMenuExecute(currentState);
            break;
        default:
            break;
        }
    }
}