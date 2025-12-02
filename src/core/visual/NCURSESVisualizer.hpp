#pragma once
#include <ncurses.h>
#include <string>
#include <vector>
#include <memory>
#include <chrono>

#include "IVisualizer.hpp"
#include "../InputController.hpp"
#include "../IGameController.hpp"
#include "../Config.hpp"

class NCURSESVisualizer : public IVisualizer {
public:
    NCURSESVisualizer();
    ~NCURSESVisualizer();
    void setInputController(InputController* ic);
    void setGameController(IGameController* gc);
    void display();
private:
    enum class State { MainMenu, InGame, LevelUpMenu, Exit };
    State state = State::MainMenu;
    int levelup_selected = 0;

    InputController* inputController = nullptr;
    IGameController* gameController = nullptr;

    int term_w = 0;
    int term_h = 0;

    std::vector<std::string> main_menu_items = { "New Game", "Continue Game", "Exit" };
    int main_menu_selected = 0;

    const int frame_ms = 15;
    std::chrono::steady_clock::time_point last_frame_time;

    void initCurses();
    void deinitCurses();
    void updateTermSize();

    void loopMainMenu();
    void loopInGame();
    void loopLevelUp();

    void drawMainMenu();
    void drawInGame();
    void drawLevelUpMenu();

    int fetchInput();

    void drawLeftPanel(int x, int y, int w, int h);
    void drawFieldPanel(int x, int y, int w, int h);
    void drawRightPanel(int x, int y, int w);

    void drawBoxTitle(int x, int y, int w, const std::string& title);
    void setColor(char out);
    void unsetColor(char out);
};
