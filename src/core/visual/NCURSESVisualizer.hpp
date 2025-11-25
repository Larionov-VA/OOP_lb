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

    // Устанавливаем контроллеры (могут быть nullptr)
    void setInputController(InputController* ic);
    void setGameController(IGameController* gc);

    // Запустить визуализацию (блокирует, пока пользователь не выйдет)
    void display();

private:
    enum class State { MainMenu, InGame, LevelUpMenu, Exit };
    State state = State::MainMenu;
    int levelup_selected = 0;
    // Ссылки на внешние контроллеры
    InputController* inputController = nullptr;
    IGameController* gameController = nullptr;

    // ncurses окна / размеры
    int term_w = 0;
    int term_h = 0;

    // меню
    std::vector<std::string> main_menu_items = { "New Game", "Continue Game", "Exit" };
    int main_menu_selected = 0;

    // Время для контроля fps
    std::chrono::steady_clock::time_point last_frame_time;

    // Основные методы
    void initCurses();
    void deinitCurses();
    void updateTermSize();

    // Рендер и обработка событий
    void loopMainMenu();
    void loopInGame();
    void loopLevelUp();

    // Вспомогательные рисовалки
    void drawMainMenu();
    void drawInGame();
    void drawLevelUpMenu();

    // Считывание ввода: сначала InputController (если задан), иначе getch()
    int fetchInput();

    // Отрисовка панелей
    void drawLeftPanel(int x, int y, int w, int h);
    void drawFieldPanel(int x, int y, int w, int h);
    void drawRightPanel(int x, int y, int w, int h);

    // Helpers
    void drawBoxTitle(int x, int y, int w, const std::string& title);
};
