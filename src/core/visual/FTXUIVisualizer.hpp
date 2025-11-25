#pragma once
#include <iostream>
#include <thread>
#include <future>
#include <memory>
#include <string>
#include <vector>

#include "ftxui/dom/elements.hpp"
#include "ftxui/screen/screen.hpp"
#include "ftxui/screen/color.hpp"
#include "ftxui/component/component.hpp"
#include "ftxui/component/screen_interactive.hpp"
#include "ftxui/component/captured_mouse.hpp"
#include "ftxui/component/component_base.hpp"
#include "ftxui/component/component_options.hpp"
#include "ftxui/component/event.hpp"

#include "../InputController.hpp"
#include "../IGameController.hpp"
#include "../Config.hpp"
#include "ASCIIAssets/ASCIIAssets.hpp"
#include "IVisualizer.hpp"


class FTXUIVisualizer : public IVisualizer{
private:
    // std::shared_ptr<ftxui::ScreenInteractive> screen_;
    ftxui::ScreenInteractive screen_;
    enum class ScreenState {
        MainMenu,
        OptionsMenu,
        InGame,
        Inventory,
        LevelUp,
        Exit
    } currentState;
    IGameController* gameController = nullptr;
    InputController* inputController = nullptr;
    struct GameOptions {
        int fieldWidth = 25;
        int fieldHeight = 25;
        std::string difficulty = "MEDIUM";
        std::vector<std::string> difficulties = {"EASY", "MEDIUM", "HARD"};
    } gameOptions;
public:
    FTXUIVisualizer();
    void setInputController(InputController* inputController) override;
    void setGameController(IGameController* gameController) override;
    void display() override;
    void refresh();
private:
    void initComponentsIfNeeded();
    ftxui::Component buildRootContainer();
    ftxui::Component buildRenderer(ftxui::Component root);
    ftxui::Component buildApp(ftxui::Component renderer);
};