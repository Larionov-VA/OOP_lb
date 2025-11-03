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

#include "ASCIIAssets/HealthPotionAsset.hpp"
#include "ASCIIAssets/FireballScrollAsset.hpp"
#include "ASCIIAssets/FirestormScrollAsset.hpp"
#include "ASCIIAssets/SummonScrollAsset.hpp"
#include "ASCIIAssets/TrapScrollAsset.hpp"
#include "ASCIIAssets/UpgradeScrollAsset.hpp"

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
        Inventory,
        LevelUp,
        Exit
    } currentState;
    IGameController* controller_ = nullptr;
    struct GameOptions {
        int fieldWidth = 25;
        int fieldHeight = 25;
        std::string difficulty = "MEDIUM";
        std::vector<std::string> difficulties = {"EASY", "MEDIUM", "HARD"};
    } gameOptions;

public:
    Visualizer();
    void setController(IGameController* controller);
    void display();
    void refreash();
private:
    void initComponentsIfNeeded(Visualizer* self);
    ftxui::Component buildRootContainer();
    ftxui::Component buildRenderer(ftxui::Component root);
    ftxui::Component buildApp(ftxui::Component renderer);
};