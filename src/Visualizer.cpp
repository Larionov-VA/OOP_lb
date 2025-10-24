#include "Visualizer.hpp"
using namespace ftxui;

static int find_difficulty_index(const std::vector<std::string>& list, const std::string& value) {
    for (size_t i = 0; i < list.size(); ++i)
        if (list[i] == value)
            return static_cast<int>(i);
    return 0;
}

Visualizer::Visualizer() {
    screen_ = std::shared_ptr<ftxui::ScreenInteractive>(
        new ftxui::ScreenInteractive(ftxui::ScreenInteractive::Fullscreen())
    );
    current_state_ = ScreenState::MainMenu;
}

Visualizer::~Visualizer() {}

void Visualizer::setController(IGameController* controller) {
    controller_ = controller;
}

void Visualizer::display() {
    // ---------- MAIN MENU ----------
    int main_selected = 0;
    std::vector<std::string> main_entries = {"NEW GAME", "CONTINUE GAME", "OPTIONS", "EXIT"};
    Component main_menu = Menu(&main_entries, &main_selected) | size(WIDTH, EQUAL, 35);
    // ---------- OPTIONS MENU ----------

    int temp_width = options_.field_width;
    int temp_height = options_.field_height;
    int diff_selected = find_difficulty_index(options_.difficulties, options_.difficulty);

    auto width_slider = Slider("Width:  ", &temp_width, 10, 25, 1);
    auto height_slider = Slider("Height: ", &temp_height, 10, 25, 1);
    auto diff_menu = Menu(&options_.difficulties, &diff_selected);

    bool options_changed = false;

    auto apply_button = Button("Apply", [&] {
        options_.field_width = temp_width;
        options_.field_height = temp_height;
        options_.difficulty = options_.difficulties[diff_selected];
        options_changed = true;
        GlobalGameConfig::fieldWidth = options_.field_width;
        GlobalGameConfig::fieldHeight = options_.field_height;
        GlobalGameConfig::difficulty = (GlobalGameConfig::difficulties)find_difficulty_index(options_.difficulties, options_.difficulty);
        current_state_ = ScreenState::MainMenu;
        screen_->PostEvent(Event::Custom);
    });

    auto cancel_button = Button("Cancel", [&] {
        temp_width = options_.field_width;
        temp_height = options_.field_height;
        diff_selected = find_difficulty_index(options_.difficulties, options_.difficulty);
        current_state_ = ScreenState::MainMenu;
        screen_->PostEvent(Event::Custom);
    });

    // --- Группа элементов настроек ---
    auto controls = Container::Vertical({
        width_slider,
        height_slider,
        diff_menu
    });

    // --- Группа кнопок ---
    auto buttons = Container::Horizontal({
        apply_button,
        cancel_button
    });

    auto exit_and_save_button = Button("Save & Exit", [&] {
        if (controller_) controller_->stopGame();
        current_state_ = ScreenState::MainMenu;
        screen_->PostEvent(Event::Custom);
    });
    auto player_info = Renderer([&] {
        std::shared_ptr<PlayerData> data = controller_->getPlayerData();
        return vbox({
            text("PLAYER INFO") | bold | center | color(Color::Green),
            separator(),
            text("Health: " + std::to_string(data->playerHealth) + "|" + std::to_string(data->playerMaxHealth)),
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
    auto player_hand = Renderer([&] {
        int countOfCarts = 0;
        int maxCount = 4;
        auto hand = [&] {

            return hbox(vbox(),
                        vbox(),
                        vbox(),
                        vbox()) | flex;
        }();
        return vbox({
            text("HAND" + std::to_string(countOfCarts) + " | " + std::to_string(maxCount)) | bold | center,
            separator(),
            hand
        }) | border;
    });
    auto enemy_info = Renderer([&] {
        auto enemy_box = [&] {
            std::vector<EnemyData> data = controller_->getEnemyData();
            std::vector<Element> rows;
            for (size_t i = 0; i < data.size(); ++i) {
                std::string line;
                line += "| Enemy " + std::to_string(i) + " | ";
                line += std::to_string(data[i].enemyHealth) + "|"+ std::to_string(data[i].enemyMaxHealth) + " | ";
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
    auto in_game_container = Container::Vertical({
        player_info,
        player_hand,
        enemy_info,
        exit_and_save_button | bold | center
    });

    // --- Разделяем области фокуса: 0 — настройки, 1 — кнопки ---
    int tab_focus = 0;
    auto options_tabs = Container::Tab({
        controls,
        buttons
    }, &tab_focus);

    Component options_container = CatchEvent(options_tabs, [&](Event event) {
        if (event == Event::Tab) {
            tab_focus = (tab_focus + 1) % 2;
            return true;
        }
        if (event == Event::Escape) {
            current_state_ = ScreenState::MainMenu;
            screen_->PostEvent(Event::Custom);
            return true;
        }
        return false;
    });

    // ---------- ROOT ----------
    Component root = Container::Tab({
        main_menu,
        options_container,
        in_game_container
    }, (int*)&current_state_);

    // ---------- RENDER ----------
    Component renderer = Renderer(root, [&] {
        if (current_state_ == ScreenState::MainMenu) {
            std::string settings = "Settings: " +
                std::to_string(options_.field_width) + "x" +
                std::to_string(options_.field_height) + " - " +
                options_.difficulty;

            return vbox({
                text("MAIN MENU") | bold | center,
                separator(),
                main_menu->Render(),
                separator(),
                text(settings) | dim
            }) | border | center;
        }
        else if (current_state_ == ScreenState::InGame) {
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
                        line += nextSymbol;
                    }
                    rows.push_back(text(line) | center);
                }
                return vbox(std::move(rows)) | border;
            }();

            auto side_panel = vbox({
                in_game_container->Render()
            }) | border | size(WIDTH, EQUAL, 35);

            return hbox({
                field_box | flex,
                side_panel
            });
        }
        else if (current_state_ == ScreenState::OptionsMenu) {
            auto info = (tab_focus == 0)
                ? text("Focus: Controls (TAB to switch)") | color(Color::Yellow)
                : text("Focus: Buttons (TAB to switch)") | color(Color::Cyan);

            return vbox({
                text("OPTIONS") | bold | center,
                separator(),
                hbox({ width_slider->Render(), text(std::to_string(temp_width)) | bold }),
                hbox({ height_slider->Render(), text(std::to_string(temp_height)) | bold }),
                separator(),
                text("Difficulty:"),
                diff_menu->Render(),
                separator(),
                hbox({
                    apply_button->Render() | flex,
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

    // ---------- EVENT HANDLING ----------
    Component app = CatchEvent(renderer, [&](Event event) {
        // MAIN MENU EVENTS
        if (current_state_ == ScreenState::MainMenu) {
            if (event == Event::Return) {
                switch (main_selected) {
                    case 0: // NEW GAME
                        if (controller_) {
                            controller_->startGame();
                        }
                        current_state_ = ScreenState::InGame;
                        screen_->PostEvent(Event::Custom);
                        return true;

                    case 1: // CONTINUE GAME
                        if (controller_) {
                            controller_->startGame();
                        }
                        return true;

                    case 2: // OPTIONS
                        current_state_ = ScreenState::OptionsMenu;
                        screen_->PostEvent(Event::Custom);
                        return true;

                    case 3: // EXIT
                        current_state_ = ScreenState::Exit;
                        screen_->ExitLoopClosure()();
                        return true;
                }
            }

            if (event == Event::Escape) {
                current_state_ = ScreenState::Exit;
                screen_->ExitLoopClosure()();
                return true;
            }
        }

        // IN GAME EVENTS
        else if (current_state_ == ScreenState::InGame) {
            if (event.is_mouse()) {
                return true;
            }
            if (event == ftxui::Event::Special("resize")) {
                return true;
            }
            if (event == Event::Escape) {
                current_state_ = ScreenState::MainMenu;
                controller_->stopGame();
                screen_->PostEvent(Event::Custom);
                return true;
            }
            if (event == Event::Character('w') || event == Event::ArrowUp) {
                controller_->performAnAction('w');
                return true;
            }
            else if (event == Event::Character('s') || event == Event::ArrowDown) {
                controller_->performAnAction('s');
                return true;
            }
            else if (event == Event::Character('a') || event == Event::ArrowLeft) {
                controller_->performAnAction('a');
                return true;
            }
            else if (event == Event::Character('d') || event == Event::ArrowRight) {
                controller_->performAnAction('d');
                return true;
            }
            else if (event == Event::Character('q')) {
                controller_->performAnAction('q');
                return true;
            }
            return false;
        }
        return false;
    });
    
    // ---------- MAIN LOOP ----------
    screen_->Loop(app);
}

void Visualizer::refreash() {
    if (screen_) screen_->PostEvent(Event::Custom);
}
