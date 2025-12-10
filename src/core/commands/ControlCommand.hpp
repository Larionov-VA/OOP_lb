#include "Command.hpp"


class ControlCommand : public Command {
private:
    char controlCommand;
public:
    ControlCommand(char controlCommand) : controlCommand(controlCommand) {};
    ~ControlCommand() {};
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
        return "Control command: " + controlCommand;
    }
private:
    void mainMenuExecute(
        IGameController* gameController,
        GameState& currentState,
        GameState& prevState,
        int* selector
    );
    void loadMenuExecute(
        IGameController* gameController,
        GameState& currentState,
        GameState& prevState,
        int* selector,
        std::vector<std::string> loadMenuOptions,
        int* startList,
        int* endList,
        std::string* errMessage,
        bool* firstCall
    );
    void inGameExecute(
        GameState& currentState
    );
    void levelUpMenuExecute(
        IGameController* gameController,
        GameState& currentState,
        int* selector
    );
    void gameOverExecute(
        IGameController* gameController,
        GameState& currentState,
        GameState& prevState,
        int* selector
    );
    void pauseMenuExecute(
        IGameController* gameController,
        GameState& currentState,
        GameState& prevState,
        int* selector
    );
    void autorsMenuExecute(
        GameState& currentState
    );
};

