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
    Component main_menu = Menu(&main_entries, &main_selected);

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
        // Shift+Tab (обычно генерируется как ESC [ Z)
        if (event.is_character() && event.character() == "\033[Z") {
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
        options_container
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
                text(settings) | color(Color::Green) | dim
            }) | border | center;
        }
        else if (current_state_ == ScreenState::InGame) {
            // --- поле ---
            auto field_box = [&] {
                std::vector<Element> rows;
                for (int y = 0; y < GlobalGameConfig::fieldHeight; ++y) {
                    std::string line;
                    for (int x = 0; x < GlobalGameConfig::fieldWidth; ++x) {
                        int idx = y * GlobalGameConfig::fieldWidth + x;
                        line += (idx < (int)fieldChars.size()) ? fieldChars[idx]: ' ';
                        line += " ";
                    }
                    rows.push_back(text(line) | center);
                }
                return vbox(std::move(rows)) | border;
            }();

            // --- боковая панель ---
            auto quit_button = Button("Quit & Save", [&] {
                // if (controller_) controller_->saveAndQuit();
                current_state_ = ScreenState::MainMenu;
                screen_->PostEvent(Event::Custom);
            });

            auto side_panel = vbox({
                text("PLAYER INFO") | bold,
                separator(),
                text("Level: " + std::to_string(GlobalGameConfig::gameLevel)),
                separator(),
                quit_button->Render()   // ✅ теперь Render() превращает компонент в Element
            }) | border | size(WIDTH, EQUAL, 25);

            // --- финальный layout ---
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
        if (current_state_ == ScreenState::MainMenu) {
            if (event == Event::Return) {
                switch (main_selected) {
                    case 0: 
                        if (controller_) {
                            controller_->startGame();
                        }
                        current_state_ = ScreenState::InGame;
                        screen_->PostEvent(Event::Custom);
                        return true;
                    case 1: 
                        if (controller_) {
                            controller_->startGame();
                        }
                        return true;
                    case 2:
                        current_state_ = ScreenState::OptionsMenu;
                        screen_->PostEvent(Event::Custom);
                        return true;
                    case 3:
                        current_state_ = ScreenState::Exit;
                        screen_->ExitLoopClosure()();
                        return true;
                }
            }
            if (event == Event::Character('q')) {
                current_state_ = ScreenState::Exit;
                screen_->ExitLoopClosure()();
                return true;
            }
        }
        return false;
    });

    // ---------- MAIN LOOP ----------
    screen_->Loop(app);
}

void Visualizer::refreash() {
    if (screen_) screen_->PostEvent(Event::Custom);
}
