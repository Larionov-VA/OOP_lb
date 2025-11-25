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


FTXUIVisualizer::FTXUIVisualizer()
    : screen_(ftxui::ScreenInteractive::Fullscreen())  // Инициализация в списке инициализации
{
    currentState = ScreenState::MainMenu;
    inputController = nullptr;
    gameController = nullptr;
}


void FTXUIVisualizer::setGameController(IGameController* controller) {
    gameController = controller;
}

void FTXUIVisualizer::setInputController(InputController* controller) {
    inputController = controller;
}

void FTXUIVisualizer::initComponentsIfNeeded() {
    if (componentsInitialized) return;
    componentsInitialized = true;

    mainSelected = 0;
    mainEntries = {"NEW GAME", "CONTINUE GAME", "OPTIONS", "EXIT"};

    tempWidth = this->gameOptions.fieldWidth;
    tempHeight = this->gameOptions.fieldHeight;
    diffSelected = findDifficultyIndex(
        this->gameOptions.difficulties, this->gameOptions.difficulty);

    mainMenuComp = Menu(&mainEntries, &mainSelected);

    widthSliderComp = Slider("Width:  ", &tempWidth, 10, 25, 1);
    heightSliderComp = Slider("Height: ", &tempHeight, 10, 25, 1);
    diffMenuComp = Menu(&this->gameOptions.difficulties, &diffSelected);

    applyButtonComp = Button("Apply", [this] {
        this->gameOptions.fieldWidth = tempWidth;
        this->gameOptions.fieldHeight = tempHeight;
        this->gameOptions.difficulty = this->gameOptions.difficulties[diffSelected];
        optionsChanged = true;
        GlobalGameConfig::fieldWidth = this->gameOptions.fieldWidth;
        GlobalGameConfig::fieldHeight = this->gameOptions.fieldHeight;
        GlobalGameConfig::difficulty = findDifficultyIndex(
            this->gameOptions.difficulties, this->gameOptions.difficulty);
        this->currentState = ScreenState::MainMenu;
        this->refresh();
    });

    cancelButtonComp = Button("Cancel", [this] {
        tempWidth = this->gameOptions.fieldWidth;
        tempHeight = this->gameOptions.fieldHeight;
        diffSelected = findDifficultyIndex(
            this->gameOptions.difficulties, this->gameOptions.difficulty);
        this->currentState = ScreenState::MainMenu;
        this->refresh();
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

    exitAndSaveButtonComp = Button("Save & Exit", [this] {
        if (this->gameController) {
            this->gameController->saveGame();
            this->gameController->stopGame();
        }
        this->currentState = ScreenState::MainMenu;
        this->refresh();
    });

    playerInfoComp = Renderer([this] {
        std::shared_ptr<PlayerData> data = this->gameController->getPlayerData();
        if (data->levelIncreased) {
            this->currentState = ScreenState::LevelUp;
        }
        float lifeIndicator = (float)data->playerHealth/data->playerMaxHealth;
        float currentExp = data->playerCurrentExperience - data->playerPrevLevelUpExperience;
        float expToLvlUp = data->playerLevelUpExperience - data->playerPrevLevelUpExperience;
        float expIndicator = currentExp/expToLvlUp;
        return vbox({
            text("PLAYER INFO"),
            separator(),
            gauge(lifeIndicator),
            text("Health: " + std::to_string(data->playerHealth) +
                         "/" + std::to_string(data->playerMaxHealth)),
            gauge(expIndicator),
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
        });
    });

    playerInventoryComp = Renderer([this] {
        return vbox({
            text("INVENTORY"),
            separator(),
        });
    });

    levelUpChoiceComp = Renderer([this] {
        std::shared_ptr<PlayerData> data = this->gameController->getPlayerData();
        return vbox({
            text("Level Up! " + std::to_string(data->playerLevel) + " Level"),
            separator(),
            hbox(
                vbox(text("int: " + std::to_string(data->playerIntelligence) + " + 10"), text("to choice PRESS '1'")),
                vbox(text("str: " + std::to_string(data->playerStrength) + " + 10"), text("to choice PRESS '2'")),
                vbox(text("dex: " + std::to_string(data->playerDexterity) + " + 10"), text("to choice PRESS '3'"))
            )
        });
    });

    playerHandComp = Renderer([this] {
        std::shared_ptr<PlayerData> data = this->gameController->getPlayerData();

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
                    rows.push_back(text(line));
                } else {
                    rows.push_back(text(line));
                }
            }

            return vbox({
                paragraph(inHand),
                text("amount: " + std::to_string(inHandCount)),
                separator(),
                vbox(std::move(rows))
            });
        }();

        return vbox({
            text("HAND " + std::to_string(data->playerCurrentHandSize) + "/" +
                        std::to_string(data->playerMaxHandSize)),
            separator(),
            hand
        });
    });

    enemyInfoComp = Renderer([this] {
        auto enemy_box = [&] {
            std::vector<EnemyData> data = this->gameController->getEnemyData();
            std::vector<Element> rows;
            for (size_t i = 0; i < data.size(); ++i) {
                std::string line;
                line += data[i].name + " | ";
                line += std::to_string(data[i].enemyHealth) + "/" +
                        std::to_string(data[i].enemyMaxHealth) + " | ";
                line += std::to_string(data[i].enemyAttack);
                rows.push_back(text(line));
            }
            return vbox(std::move(rows));
        }();
        return vbox({
            text("ENEMY INFO"),
            separator(),
            enemy_box
        });
    });

    leftInGameContainerComp = Container::Vertical({
        playerInfoComp,
        enemyInfoComp
    });

    rigthInGameContainerComp = Container::Vertical({
        playerHandComp,
        exitAndSaveButtonComp
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
    initComponentsIfNeeded();
    return Container::Tab({
        mainMenuComp,
        optionsContainerComp,
        rigthInGameContainerComp,
        leftInGameContainerComp
    }, (int*)&currentState);
}


ftxui::Component FTXUIVisualizer::buildRenderer(ftxui::Component root) {
    initComponentsIfNeeded();
    return Renderer(root, [&] {
        if (currentState == ScreenState::MainMenu) {
            std::string settings = "Settings: " +
                std::to_string(gameOptions.fieldWidth) + "x" +
                std::to_string(gameOptions.fieldHeight) + " - " +
                gameOptions.difficulty;
            return vbox({
                text("MAIN MENU"),
                separator(),
                mainMenuComp->Render(),
                separator(),
                text(settings)
            });
        }
        else if (currentState == ScreenState::InGame) {
            auto field_box = [&] {
                std::vector<wchar_t> fieldChars = gameController->getFieldData();
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
                    rows.push_back(text(line));
                }
                return vbox(std::move(rows));
            }();
            auto leftSidePanel = vbox({
                leftInGameContainerComp->Render()
            });
            auto rigthSidePanel = vbox({
                rigthInGameContainerComp->Render()
            });
            return hbox({
                rigthSidePanel,
                field_box,
                leftSidePanel
            });
        }
        else if (currentState == ScreenState::Inventory) {
            return vbox({
                playerInventoryComp->Render()
            });
        }
        else if (currentState == ScreenState::LevelUp) {
            return vbox({
                levelUpChoiceComp->Render()
            });
        }
        else if (currentState == ScreenState::OptionsMenu) {
            auto info = (tabFocus == 0)
                ? text("Focus: Controls (TAB to switch)")
                : text("Focus: Buttons (TAB to switch)");
            return vbox({
                text("OPTIONS"),
                separator(),
                hbox({ widthSliderComp->Render(), text(std::to_string(tempWidth)) }),
                hbox({ heightSliderComp->Render(), text(std::to_string(tempHeight)) }),
                separator(),
                text("Difficulty:"),
                diffMenuComp->Render(),
                separator(),
                hbox({
                    applyButtonComp->Render(),
                    cancelButtonComp->Render()
                }),
                separator(),
                info
            });
        }
        else {
            return text("Exiting...");
        }
    });
}

ftxui::Component FTXUIVisualizer::buildApp(ftxui::Component renderer) {
    initComponentsIfNeeded();
    return CatchEvent(renderer, [&](Event event) {
        if (currentState == ScreenState::MainMenu) {
            if (event == Event::Return) {
                switch (mainSelected) {
                    case 0: // NEW GAME
                        if (gameController) {
                            gameController->startNewGame();
                        }
                        currentState = ScreenState::InGame;
                        refresh();
                        return true;

                    case 1: // CONTINUE GAME
                        if (gameController) {
                            gameController->startNewGame();
                        }
                        return true;

                    case 2: // OPTIONS
                        currentState = ScreenState::OptionsMenu;
                        refresh();
                        return true;

                    case 3: // EXIT
                        currentState = ScreenState::Exit;
                        if (screen_.Active()) screen_.ExitLoopClosure()();
                        return true;
                }
            }
            if (event.is_mouse()) {
                return true;
            }
            if (event == Event::Escape) {
                currentState = ScreenState::Exit;
                if (screen_.Active()) screen_.ExitLoopClosure()();
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
                refresh();
                return true;
            }
            if (event == Event::Escape) {
                currentState = ScreenState::MainMenu;
                if (gameController) gameController->stopGame();
                refresh();
                return true;
            }
            if (event == Event::Character('w') || event == Event::ArrowUp) {
                if (!gameController->performAnAction('w')) {
                    currentState = ScreenState::MainMenu;
                    gameController->stopGame();
                    refresh();
                }
                return true;
            }
            else if (event == Event::Character('s') || event == Event::ArrowDown) {
                if (!gameController->performAnAction('s')) {
                    currentState = ScreenState::MainMenu;
                    gameController->stopGame();
                    refresh();
                }
                return true;
            }
            else if (event == Event::Character('a') || event == Event::ArrowLeft) {
                if (!gameController->performAnAction('a')) {
                    currentState = ScreenState::MainMenu;
                    gameController->stopGame();
                    refresh();
                }
                return true;
            }
            else if (event == Event::Character('d') || event == Event::ArrowRight) {
                if (!gameController->performAnAction('d')) {
                    currentState = ScreenState::MainMenu;
                    gameController->stopGame();
                    refresh();
                }
                return true;
            }
            else if (event == Event::Character('e')) {
                if (!gameController->performAnAction('e')) {
                    currentState = ScreenState::MainMenu;
                    gameController->stopGame();
                    refresh();
                }
                return true;
            }
            else if (event == Event::Character('q')) {
                gameController->performAnAction('q');
                return true;
            }
            else if (event == Event::Character('1')) {
                gameController->performAnAction('0');
                return true;
            }
            else if (event == Event::Character('2')) {
                gameController->performAnAction('1');
                return true;
            }
            else if (event == Event::Character('3')) {
                gameController->performAnAction('2');
                return true;
            }
            else if (event == Event::Character('4')) {
                gameController->performAnAction('3');
                return true;
            }
            return false;
        }
        else if (currentState == ScreenState::Inventory) {
            if (event == Event::Escape) {
                currentState = ScreenState::InGame;
                refresh();
                return true;
            }
        }
        else if (currentState == ScreenState::LevelUp) {
            if (event == Event::Escape) {
                currentState = ScreenState::InGame;
                refresh();
                return true;
            }
            if (event == Event::Character('1')) {
                gameController->playerLevelUp('1');
                currentState = ScreenState::InGame;
                return true;
            }
            if (event == Event::Character('2')) {
                gameController->playerLevelUp('2');
                currentState = ScreenState::InGame;
                return true;
            }
            if (event == Event::Character('3')) {
                gameController->playerLevelUp('3');
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
    initComponentsIfNeeded();
    Component root = buildRootContainer();
    Component renderer = buildRenderer(root);
    Component app = buildApp(renderer);
    screen_.Loop(app);
}


void FTXUIVisualizer::refresh() {
    if (screen_.Active()) screen_.PostEvent(Event::Custom);
}