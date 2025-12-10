#include "Command.hpp"


class PlayerActionCommand : public Command {
private:
    char actionSymbol;
public:
    PlayerActionCommand(char actionSymbol) : actionSymbol(actionSymbol) {};
    ~PlayerActionCommand() {};
    void execute(
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
    ) override;
    std::string getCommandName() override {
        return "Action command: " + actionSymbol;
    }

};
