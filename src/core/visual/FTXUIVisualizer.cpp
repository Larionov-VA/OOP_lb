#include "FTXUIVisualizer.hpp"
using namespace ftxui;


namespace {
    static int mainSelected = 0;
    static std::vector<std::string> mainEntries = {"NEW GAME", "CONTINUE GAME", "OPTIONS", "EXIT"};
    static int tempWidth = 25;
    static int tempHeight = 25;
    static int diffSelected = 0;
    static bool optionsChanged = false;
    static int tabFocus = 0;
    static ftxui::Component mainMenuComp;
    static ftxui::Component widthSliderComp;
    static ftxui::Component heightSliderComp;
    static ftxui::Component diffMenuComp;
    static ftxui::Component applyButtonComp;
    static ftxui::Component cancelButtonComp;
    static ftxui::Component controlsComp;
    static ftxui::Component buttonsComp;
    static ftxui::Component exitAndSaveButtonComp;
    static ftxui::Component playerInfoComp;
    static ftxui::Component playerInventoryComp;
    static ftxui::Component levelUpChoiceComp;
    static ftxui::Component playerHandComp;
    static ftxui::Component enemyInfoComp;
    static ftxui::Component leftInGameContainerComp;
    static ftxui::Component rigthInGameContainerComp;
    static ftxui::Component optionsTabsComp;
    static ftxui::Component optionsContainerComp;

    static bool componentsInitialized = false;
}


static int findDifficultyIndex(const std::vector<std::string>& list, const std::string& value) {
    for (size_t i = 0; i < list.size(); ++i)
        if (list[i] == value)
            return static_cast<int>(i);
    return 0;
}


FTXUIVisualizer::FTXUIVisualizer() {
    screen_ = std::shared_ptr<ftxui::ScreenInteractive>(
        new ftxui::ScreenInteractive(ftxui::ScreenInteractive::Fullscreen())
    );
    currentState = ScreenState::MainMenu;
}


void FTXUIVisualizer::setController(IGameController* controller) {
    controller_ = controller;
}


void FTXUIVisualizer::initComponentsIfNeeded(FTXUIVisualizer* self) {
    if (componentsInitialized) return;
    componentsInitialized = true;

    mainSelected = 0;
    mainEntries = {"NEW GAME", "CONTINUE GAME", "OPTIONS", "EXIT"};

    tempWidth = self->gameOptions.fieldWidth;
    tempHeight = self->gameOptions.fieldHeight;
    diffSelected = findDifficultyIndex(
        self->gameOptions.difficulties, self->gameOptions.difficulty);

    mainMenuComp = Menu(&mainEntries, &mainSelected) | size(WIDTH, EQUAL, 35);

    widthSliderComp = Slider("Width:  ", &tempWidth, 10, 25, 1);
    heightSliderComp = Slider("Height: ", &tempHeight, 10, 25, 1);
    diffMenuComp = Menu(&self->gameOptions.difficulties, &diffSelected);

    applyButtonComp = Button("Apply", [self] {
        self->gameOptions.fieldWidth = tempWidth;
        self->gameOptions.fieldHeight = tempHeight;
        self->gameOptions.difficulty = self->gameOptions.difficulties[diffSelected];
        optionsChanged = true;
        GlobalGameConfig::fieldWidth = self->gameOptions.fieldWidth;
        GlobalGameConfig::fieldHeight = self->gameOptions.fieldHeight;
        GlobalGameConfig::difficulty = findDifficultyIndex(
            self->gameOptions.difficulties, self->gameOptions.difficulty);
        self->currentState = ScreenState::MainMenu;
        self->refreash();
    });

    cancelButtonComp = Button("Cancel", [self] {
        tempWidth = self->gameOptions.fieldWidth;
        tempHeight = self->gameOptions.fieldHeight;
        diffSelected = findDifficultyIndex(
            self->gameOptions.difficulties, self->gameOptions.difficulty);
        self->currentState = ScreenState::MainMenu;
        self->refreash();
    });

    controlsComp = Container::Vertical({
        widthSliderComp,
        heightSliderComp,
        diffMenuComp
    });

    buttonsComp = Container::Horizontal({
        applyButtonComp,
        cancelButtonComp
    });

    exitAndSaveButtonComp = Button("Save & Exit", [self] {
        if (self->controller_) {
            self->controller_->saveGame();
            self->controller_->stopGame();
        }
        self->currentState = ScreenState::MainMenu;
        self->refreash();
    });

    playerInfoComp = Renderer([self] {
        std::shared_ptr<PlayerData> data = self->controller_->getPlayerData();
        if (data->levelIncreased) {
            self->currentState = ScreenState::LevelUp;
        }
        float lifeIndicator = (float)data->playerHealth/data->playerMaxHealth;
        float currentExp = data->playerCurrentExperience - data->playerPrevLevelUpExperience;
        float expToLvlUp = data->playerLevelUpExperience - data->playerPrevLevelUpExperience;
        float expIndicator = currentExp/expToLvlUp;
        return vbox({
            text("PLAYER INFO") | bold | center | color(Color::Green),
            separator(),
            gauge(lifeIndicator) | color(Color::Red) | border | flex,
            text("Health: " + std::to_string(data->playerHealth) +
                         "/" + std::to_string(data->playerMaxHealth)),
            gauge(expIndicator) | color(Color::Blue) | border | flex,
            text("Experience: " + std::to_string(data->playerCurrentExperience)
                        + "/" + std::to_string(data->playerLevelUpExperience)),
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

    playerInventoryComp = Renderer([self] {
        return vbox({
            text("INVENTORY") | bold | center,
            separator(),
        });
    });

    levelUpChoiceComp = Renderer([self] {
        std::shared_ptr<PlayerData> data = self->controller_->getPlayerData();
        return vbox({
            text("Level Up! " + std::to_string(data->playerLevel) + " Level") | bold | center,
            separator(),
            hbox(
                vbox(text("int: " + std::to_string(data->playerIntelligence) + " + 10") |
                 color(Color::Blue), text("to choice PRESS '1'") | center) | border,
                vbox(text("str: " + std::to_string(data->playerStrength) + " + 10") |
                 color(Color::Red), text("to choice PRESS '2'") | center) | border,
                vbox(text("dex: " + std::to_string(data->playerDexterity) + " + 10") |
                 color(Color::Green), text("to choice PRESS '3'") | center) | border
            )
        });
    });
    playerHandComp = Renderer([self] {
        std::shared_ptr<PlayerData> data = self->controller_->getPlayerData();

        struct SpellInfo {
            const std::string& asciiArt;
            const std::string name;
        };

        const std::array<SpellInfo, 4> spells = {{
            {firestormScrollASCII, "AOE Spell"},
            {fireballScrollASCII, "Fireball Spell"},
            {upgradeSpellASCII, "Upgrade Spell"},
            {trapSpellASCII, "Trap Spell"}
        }};

        auto hand = [&] {
            std::string inHand;
            int inHandCount = 0;
            std::vector<Element> rows;

            for (int i = 0; i < 4; ++i) {
                const auto& item = data->playerHandItem[i];
                bool isSelected = item.first;
                int count = item.second;

                std::string line = std::to_string(i+1) + ": " + spells[i].name + "\t| " + std::to_string(count);

                if (!isSelected) {
                    inHand = spells[i].asciiArt;
                    inHandCount = count;
                    rows.push_back(text(line) | center | dim);
                } else {
                    rows.push_back(text(line) | center);
                }
            }

            return vbox({
                paragraph(inHand) | center | border | flex,
                text("amount: " + std::to_string(inHandCount)),
                separator(),
                vbox(std::move(rows)) | center
            });
        }();

        return vbox({
            text("HAND " + std::to_string(data->playerCurrentHandSize) + "/" +
                        std::to_string(data->playerMaxHandSize)) | bold | center,
            separator(),
            hand
        });
    });

    enemyInfoComp = Renderer([self] {
        auto enemy_box = [&] {
            std::vector<EnemyData> data = self->controller_->getEnemyData();
            std::vector<Element> rows;
            for (size_t i = 0; i < data.size(); ++i) {
                std::string line;
                line += data[i].name + " | ";
                line += std::to_string(data[i].enemyHealth) + "/" +
                        std::to_string(data[i].enemyMaxHealth) + " | ";
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

    leftInGameContainerComp = Container::Vertical({
        playerInfoComp,
        enemyInfoComp
    });

    rigthInGameContainerComp = Container::Vertical({
        playerHandComp,
        exitAndSaveButtonComp | bold | center
    });

    optionsTabsComp = Container::Tab({
        controlsComp,
        buttonsComp
    }, &tabFocus);

    optionsContainerComp = CatchEvent(optionsTabsComp, [](Event event) {
        if (event == Event::Tab) {
            tabFocus = (tabFocus + 1) % 2;
            return true;
        }
        if (event == Event::Escape) {
            return true;
        }
        if (event.is_mouse()) {
            return true;
        }
        return false;
    });
}


ftxui::Component FTXUIVisualizer::buildRootContainer() {
    initComponentsIfNeeded(this);
    return Container::Tab({
        mainMenuComp,
        optionsContainerComp,
        rigthInGameContainerComp,
        leftInGameContainerComp
    }, (int*)&currentState);
}


ftxui::Component FTXUIVisualizer::buildRenderer(ftxui::Component root) {
    initComponentsIfNeeded(this);
    return Renderer(root, [&] {
        if (currentState == ScreenState::MainMenu) {
            std::string settings = "Settings: " +
                std::to_string(gameOptions.fieldWidth) + "x" +
                std::to_string(gameOptions.fieldHeight) + " - " +
                gameOptions.difficulty;
            return vbox({
                text("MAIN MENU") | bold | center,
                separator(),
                mainMenuComp->Render(),
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
                leftInGameContainerComp->Render()
            }) | border | size(WIDTH, EQUAL, 45);
            auto rigthSidePanel = vbox({
                rigthInGameContainerComp->Render()
            }) | border | size(WIDTH, EQUAL, 45);
            return hbox({
                rigthSidePanel,
                field_box,
                leftSidePanel
            });
        }
        else if (currentState == ScreenState::Inventory) {
            return vbox({
                playerInventoryComp->Render()
            }) | border | center;
        }
        else if (currentState == ScreenState::LevelUp) {
            return vbox({
                levelUpChoiceComp->Render()
            }) | border | center;
        }
        else if (currentState == ScreenState::OptionsMenu) {
            auto info = (tabFocus == 0)
                ? text("Focus: Controls (TAB to switch)") | color(Color::Yellow)
                : text("Focus: Buttons (TAB to switch)") | color(Color::Cyan);
            return vbox({
                text("OPTIONS") | bold | center,
                separator(),
                hbox({ widthSliderComp->Render(), text(std::to_string(tempWidth)) | bold }),
                hbox({ heightSliderComp->Render(), text(std::to_string(tempHeight)) | bold }),
                separator(),
                text("Difficulty:"),
                diffMenuComp->Render(),
                separator(),
                hbox({
                    applyButtonComp->Render() | flex,
                    cancelButtonComp->Render() | flex
                }) | center,
                separator(),
                info | center
            }) | border | center;
        }
        else {
            return text("Exiting...") | center;
        }
    });
}

ftxui::Component FTXUIVisualizer::buildApp(ftxui::Component renderer) {
    initComponentsIfNeeded(this);
    return CatchEvent(renderer, [&](Event event) {
        if (currentState == ScreenState::MainMenu) {
            if (event == Event::Return) {
                switch (mainSelected) {
                    case 0: // NEW GAME
                        if (controller_) {
                            controller_->startNewGame();
                        }
                        currentState = ScreenState::InGame;
                        refreash();
                        return true;

                    case 1: // CONTINUE GAME
                        if (controller_) {
                            controller_->startNewGame();
                        }
                        return true;

                    case 2: // OPTIONS
                        currentState = ScreenState::OptionsMenu;
                        refreash();
                        return true;

                    case 3: // EXIT
                        currentState = ScreenState::Exit;
                        if (screen_) screen_->ExitLoopClosure()();
                        return true;
                }
            }
            if (event.is_mouse()) {
                return true;
            }
            if (event == Event::Escape) {
                currentState = ScreenState::Exit;
                if (screen_) screen_->ExitLoopClosure()();
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
                refreash();
                return true;
            }
            if (event == Event::Escape) {
                currentState = ScreenState::MainMenu;
                if (controller_) controller_->stopGame();
                refreash();
                return true;
            }
            if (event == Event::Character('w') || event == Event::ArrowUp) {
                if (!controller_->performAnAction('w')) {
                    currentState = ScreenState::MainMenu;
                    controller_->stopGame();
                    refreash();
                }
                return true;
            }
            else if (event == Event::Character('s') || event == Event::ArrowDown) {
                if (!controller_->performAnAction('s')) {
                    currentState = ScreenState::MainMenu;
                    controller_->stopGame();
                    refreash();
                }
                return true;
            }
            else if (event == Event::Character('a') || event == Event::ArrowLeft) {
                if (!controller_->performAnAction('a')) {
                    currentState = ScreenState::MainMenu;
                    controller_->stopGame();
                    refreash();
                }
                return true;
            }
            else if (event == Event::Character('d') || event == Event::ArrowRight) {
                if (!controller_->performAnAction('d')) {
                    currentState = ScreenState::MainMenu;
                    controller_->stopGame();
                    refreash();
                }
                return true;
            }
            else if (event == Event::Character('e')) {
                if (!controller_->performAnAction('e')) {
                    currentState = ScreenState::MainMenu;
                    controller_->stopGame();
                    refreash();
                }
                return true;
            }
            else if (event == Event::Character('q')) {
                controller_->performAnAction('q');
                return true;
            }
            else if (event == Event::Character('1')) {
                controller_->performAnAction('0');
                return true;
            }
            else if (event == Event::Character('2')) {
                controller_->performAnAction('1');
                return true;
            }
            else if (event == Event::Character('3')) {
                controller_->performAnAction('2');
                return true;
            }
            else if (event == Event::Character('4')) {
                controller_->performAnAction('3');
                return true;
            }
            return false;
        }
        else if (currentState == ScreenState::Inventory) {
            if (event == Event::Escape) {
                currentState = ScreenState::InGame;
                refreash();
                return true;
            }
        }
        else if (currentState == ScreenState::LevelUp) {
            if (event == Event::Escape) {
                currentState = ScreenState::InGame;
                refreash();
                return true;
            }
            if (event == Event::Character('1')) {
                controller_->playerLevelUp('1');
                currentState = ScreenState::InGame;
                return true;
            }
            if (event == Event::Character('2')) {
                controller_->playerLevelUp('2');
                currentState = ScreenState::InGame;
                return true;
            }
            if (event == Event::Character('3')) {
                controller_->playerLevelUp('3');
                currentState = ScreenState::InGame;
                return true;
            }
        }
        else if (currentState == ScreenState::OptionsMenu) {
            if (event == Event::Escape) {
                currentState = ScreenState::MainMenu;
                return true;
            }
        }
        return false;
    });
}


void FTXUIVisualizer::display() {
    initComponentsIfNeeded(this);
    Component root = buildRootContainer();
    Component renderer = buildRenderer(root);
    Component app = buildApp(renderer);
    screen_->Loop(app);
}


void FTXUIVisualizer::refreash() {
    if (screen_) screen_->PostEvent(Event::Custom);
}