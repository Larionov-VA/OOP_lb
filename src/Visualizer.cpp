#include "Visualizer.hpp"
using namespace ftxui;

static int findDifficultyIndex(const std::vector<std::string>& list, const std::string& value) {
    for (size_t i = 0; i < list.size(); ++i)
        if (list[i] == value)
            return static_cast<int>(i);
    return 0;
}

Visualizer::Visualizer() {
    screen_ = std::shared_ptr<ftxui::ScreenInteractive>(
        new ftxui::ScreenInteractive(ftxui::ScreenInteractive::Fullscreen())
    );
    currentState = ScreenState::MainMenu;
}

Visualizer::~Visualizer() {}

void Visualizer::setController(IGameController* controller) {
    controller_ = controller;
}

void Visualizer::display() {
    // ---------- MAIN MENU ----------
    int mainSelected = 0;
    std::vector<std::string> mainEntries = {"NEW GAME", "CONTINUE GAME", "OPTIONS", "EXIT"};
    Component mainMenu = Menu(&mainEntries, &mainSelected) | size(WIDTH, EQUAL, 35);
    // ---------- OPTIONS MENU ----------

    int tempWidth = gameOptions.fieldWidth;
    int tempHeight = gameOptions.fieldHeight;
    int diffSelected = findDifficultyIndex(gameOptions.difficulties, gameOptions.difficulty);

    auto widthSlider = Slider("Width:  ", &tempWidth, 10, 25, 1);
    auto heightSlider = Slider("Height: ", &tempHeight, 10, 25, 1);
    auto diffMenu = Menu(&gameOptions.difficulties, &diffSelected);

    bool optionsChanged = false;

    auto applyButton = Button("Apply", [&] {
        gameOptions.fieldWidth = tempWidth;
        gameOptions.fieldHeight = tempHeight;
        gameOptions.difficulty = gameOptions.difficulties[diffSelected];
        optionsChanged = true;
        GlobalGameConfig::fieldWidth = gameOptions.fieldWidth;
        GlobalGameConfig::fieldHeight = gameOptions.fieldHeight;
        GlobalGameConfig::difficulty = (GlobalGameConfig::difficulties)findDifficultyIndex(gameOptions.difficulties, gameOptions.difficulty);
        currentState = ScreenState::MainMenu;
        screen_->PostEvent(Event::Custom);
    });

    auto cancel_button = Button("Cancel", [&] {
        tempWidth = gameOptions.fieldWidth;
        tempHeight = gameOptions.fieldHeight;
        diffSelected = findDifficultyIndex(gameOptions.difficulties, gameOptions.difficulty);
        currentState = ScreenState::MainMenu;
        screen_->PostEvent(Event::Custom);
    });

    // --- Группа элементов настроек ---
    auto controls = Container::Vertical({
        widthSlider,
        heightSlider,
        diffMenu
    });

    // --- Группа кнопок ---
    auto buttons = Container::Horizontal({
        applyButton,
        cancel_button
    });

    auto exitAndSaveButton = Button("Save & Exit", [&] {
        if (controller_) controller_->stopGame();
        currentState = ScreenState::MainMenu;
        screen_->PostEvent(Event::Custom);
    });

    auto playerInfo = Renderer([&] {
        std::shared_ptr<PlayerData> data = controller_->getPlayerData();
        float lifeIndicator = (float)data->playerHealth/data->playerMaxHealth;
        float expIndicator = (float)(data->playerCurrentExperience - data->playerPrevLevelUpExperience)/(data->playerLevelUpExperience - data->playerPrevLevelUpExperience);
        return vbox({
            text("PLAYER INFO") | bold | center | color(Color::Green),
            separator(),
            gauge(lifeIndicator) | color(Color::Red) | border | flex,
            text("Health: " + std::to_string(data->playerHealth) + "/" + std::to_string(data->playerMaxHealth)),
            gauge(expIndicator) | color(Color::Blue) | border | flex,
            text("Experience: " + std::to_string(data->playerCurrentExperience) + "/" + std::to_string(data->playerLevelUpExperience)),
            text("Level " + std::to_string(data->playerLevel)),
            text("Attack: " + std::to_string(data->playerAttack)),
            text("Weapon: " + data->playerWeapon),
            separator(),
            text("Atribures"),
            text("int: " + std::to_string(data->playerIntelligence)),
            text("str: " + std::to_string(data->playerStrength)),
            text("dex: " + std::to_string(data->playerDexterity)),
            separator(),
            text("Debaff: " + data->playerDebaff)
        }) | border;
    });

    auto playerInventory = Renderer([&] {
        // std::shared_ptr<PlayerData> data = controller_->getPlayerData();
        // float lifeIndicator = (float)data->playerHealth/data->playerMaxHealth;
        return vbox({
            text("INVENTORY") | bold | center,
            separator(),
            hbox(
                paragraph(healthPotionASCII) | border,
                paragraph(healthPotionASCII) | border 
            ),
            hbox(
                paragraph(healthPotionASCII) | border,
                paragraph(healthPotionASCII) | border
            )
        });
    });

    auto playerHand = Renderer([&] {
        // int countOfCarts = 0;
        // int maxCount = 4;
        auto hand = [&] {
            // return paragraph(healthPotionASCII) | center | border | flex;
            // return paragraph(firestormScrollASCII) | center | border | flex;
            return paragraph(fireballScrollASCII) | center | border | flex;
        }();

        return vbox({
            text("HAND") | bold | center,
            separator(),
            hand
        });
    });

    auto enemyInfo = Renderer([&] {
        auto enemy_box = [&] {
            std::vector<EnemyData> data = controller_->getEnemyData();
            std::vector<Element> rows;
            for (size_t i = 0; i < data.size(); ++i) {
                std::string line;
                line += "| Enemy " + std::to_string(i) + " | ";
                line += std::to_string(data[i].enemyHealth) + "/"+ std::to_string(data[i].enemyMaxHealth) + " | ";
                line += std::to_string(data[i].enemyAttack);
                rows.push_back(text(line) | center);
            }
            return vbox(std::move(rows)) | flex;
        }();
        return vbox({
            text("ENEMY INFO") | bold | center | color(Color::Red),
            separator(),
            enemy_box
        }) | border;
    });

    auto leftInGameContainer = Container::Vertical({
        playerInfo,
        enemyInfo
    });

    auto rigthInGameContainer = Container::Vertical({
        playerHand,
        exitAndSaveButton | bold | center
    });

    int tabFocus = 0;
    auto optionsTabs = Container::Tab({
        controls,
        buttons
    }, &tabFocus);

    Component optionsContainer = CatchEvent(optionsTabs, [&](Event event) {
        if (event == Event::Tab) {
            tabFocus = (tabFocus + 1) % 2;
            return true;
        }
        if (event == Event::Escape) {
            currentState = ScreenState::MainMenu;
            screen_->PostEvent(Event::Custom);
            return true;
        }
        if (event.is_mouse()) {
            return true;
        }
        return false;
    });

    // ---------- ROOT ----------
    Component root = Container::Tab({
        mainMenu,
        optionsContainer,
        rigthInGameContainer,
        leftInGameContainer
    }, (int*)&currentState);

    // ---------- RENDER ----------
    Component renderer = Renderer(root, [&] {
        if (currentState == ScreenState::MainMenu) {
            std::string settings = "Settings: " +
                std::to_string(gameOptions.fieldWidth) + "x" +
                std::to_string(gameOptions.fieldHeight) + " - " +
                gameOptions.difficulty;
            return vbox({
                text("MAIN MENU") | bold | center,
                separator(),
                mainMenu->Render(),
                separator(),
                text(settings) | dim
            }) | border | center;
        }
        else if (currentState == ScreenState::InGame) {
            auto field_box = [&] {
                std::vector<wchar_t> fieldChars = controller_->getFieldData();
                std::vector<Element> rows;
                for (int y = 0; y < GlobalGameConfig::fieldHeight; ++y) {
                    std::wstring line;
                    for (int x = 0; x < GlobalGameConfig::fieldWidth; ++x) {
                        int idx = y * GlobalGameConfig::fieldWidth + x;
                        wchar_t nextSymbol = L' ';
                        line += (idx < (int)fieldChars.size()) ? fieldChars[idx] : L' ';
                        if (line.back() == L'🌳' || line.back() == L'🌲') {
                            nextSymbol = L'\0';
                        }
                        else if (line.back() == L'░') {
                            nextSymbol = L'░';
                        }
                        else if (line.back() == L'🔥') {
                            nextSymbol = '\0';
                        }
                        line += nextSymbol;
                    }
                    rows.push_back(text(line) | center | color(Color::GrayLight));
                }
                return vbox(std::move(rows)) | border | flex;
            }();
            auto leftSidePanel = vbox({
                leftInGameContainer->Render()
            }) | border | size(WIDTH, EQUAL, 45);
            auto rigthSidePanel = vbox({
                rigthInGameContainer->Render()
            }) | border | size(WIDTH, EQUAL, 45);
            return hbox({
                rigthSidePanel,
                field_box,
                leftSidePanel
            });
        }
        else if (currentState == ScreenState::Inventory) {
            return vbox({
                playerInventory->Render()
            }) | border | center;
        }
        else if (currentState == ScreenState::OptionsMenu) {
            auto info = (tabFocus == 0)
                ? text("Focus: Controls (TAB to switch)") | color(Color::Yellow)
                : text("Focus: Buttons (TAB to switch)") | color(Color::Cyan);
            return vbox({
                text("OPTIONS") | bold | center,
                separator(),
                hbox({ widthSlider->Render(), text(std::to_string(tempWidth)) | bold }),
                hbox({ heightSlider->Render(), text(std::to_string(tempHeight)) | bold }),
                separator(),
                text("Difficulty:"),
                diffMenu->Render(),
                separator(),
                hbox({
                    applyButton->Render() | flex,
                    cancel_button->Render() | flex
                }) | center,
                separator(),
                info | center
            }) | border | center;
        }
        else {
            return text("Exiting...") | center;
        }
    });

    Component app = CatchEvent(renderer, [&](Event event) {
        if (currentState == ScreenState::MainMenu) {
            if (event == Event::Return) {
                switch (mainSelected) {
                    case 0: // NEW GAME
                        if (controller_) {
                            controller_->startGame();
                        }
                        currentState = ScreenState::InGame;
                        screen_->PostEvent(Event::Custom);
                        return true;

                    case 1: // CONTINUE GAME
                        if (controller_) {
                            controller_->startGame();
                        }
                        return true;

                    case 2: // OPTIONS
                        currentState = ScreenState::OptionsMenu;
                        screen_->PostEvent(Event::Custom);
                        return true;

                    case 3: // EXIT
                        currentState = ScreenState::Exit;
                        screen_->ExitLoopClosure()();
                        return true;
                }
            }
            if (event.is_mouse()) {
                return true;
            }
            if (event == Event::Escape) {
                currentState = ScreenState::Exit;
                screen_->ExitLoopClosure()();
                return true;
            }
        }
        else if (currentState == ScreenState::InGame) {
            if (event.is_mouse()) {
                return true;
            }
            if (event == ftxui::Event::Special("resize")) {
                return true;
            }
            if (event == Event::Character('i') || event == Event::Character('I')) {
                currentState = ScreenState::Inventory;
                screen_->PostEvent(Event::Custom);
                return true;
            }
            if (event == Event::Escape) {
                currentState = ScreenState::MainMenu;
                controller_->stopGame();
                screen_->PostEvent(Event::Custom);
                return true;
            }
            if (event == Event::Character('w') || event == Event::ArrowUp) {
                if (controller_->performAnAction('w')) {
                    currentState = ScreenState::MainMenu;
                    controller_->stopGame();
                    screen_->PostEvent(Event::Custom);
                }
                return true;
            }
            else if (event == Event::Character('s') || event == Event::ArrowDown) {
                if (controller_->performAnAction('s')) {
                    currentState = ScreenState::MainMenu;
                    controller_->stopGame();
                    screen_->PostEvent(Event::Custom);
                }
                return true;
            }
            else if (event == Event::Character('a') || event == Event::ArrowLeft) {
                if (controller_->performAnAction('a')) {
                    currentState = ScreenState::MainMenu;
                    controller_->stopGame();
                    screen_->PostEvent(Event::Custom);
                }
                return true;
            }
            else if (event == Event::Character('d') || event == Event::ArrowRight) {
                if (controller_->performAnAction('d')) {
                    currentState = ScreenState::MainMenu;
                    controller_->stopGame();
                    screen_->PostEvent(Event::Custom);
                }
                return true;
            }
            else if (event == Event::Character('q')) {
                controller_->performAnAction('q');
                return true;
            }
            return false;
        }
        else if (currentState == ScreenState::Inventory) {
            if (event == Event::Escape) {
                currentState = ScreenState::InGame;
                screen_->PostEvent(Event::Custom);
                return true;
            }
        }
        return false;
    });

    screen_->Loop(app);
}

void Visualizer::refreash() {
    if (screen_) screen_->PostEvent(Event::Custom);
}
