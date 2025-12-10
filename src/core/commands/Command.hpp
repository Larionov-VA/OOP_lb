#pragma once
#include "../IGameController.hpp"
#include "../GameState.hpp"
#include "../KeyboardLayout.hpp"


class Command {
public:
    ~Command() = default;
    virtual void execute(
        IGameController* gameController,
        GameState& currentState,
        GameState& prevState,
        int* selector = nullptr,
        int mod = 0,
        std::vector<std::string> loadMenuOptions = {},
        int* startList = nullptr,
        int* endList = nullptr,
        std::string* errMessage = nullptr,
        bool* firstCall = nullptr
    ) = 0;
    virtual std::string getCommandName() = 0;
};
