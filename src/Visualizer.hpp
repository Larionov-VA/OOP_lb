#pragma once
#include "ftxui/dom/elements.hpp"
#include "ftxui/screen/screen.hpp"
#include "ftxui/screen/color.hpp"
#include "ftxui/component/component.hpp"
#include "ftxui/component/screen_interactive.hpp"
#include "ftxui/component/captured_mouse.hpp"
#include "ftxui/component/component_base.hpp"
#include "ftxui/component/component_options.hpp"
#include "ftxui/component/event.hpp"
#include "IGameController.hpp"
#include "Config.hpp"
#include <iostream>
#include <thread>
#include <future>
#include <memory>
#include <string>
#include <vector>

class Visualizer {
private:
    std::shared_ptr<ftxui::ScreenInteractive> screen_;
    enum class ScreenState {
        MainMenu,
        OptionsMenu,
        InGame,
        Exit
    } current_state_;
    IGameController* controller_ = nullptr;
    struct GameOptions {
        int field_width = 25;
        int field_height = 25;
        std::string difficulty = "MEDIUM";
        std::vector<std::string> difficulties = {"EASY", "MEDIUM", "HARD"};
    } options_;

public:
    Visualizer();
    ~Visualizer();
    void setController(IGameController* controller);
    void display();
    void refreash();
};