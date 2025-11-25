#include "NCURSESVisualizer.hpp"

NCURSESVisualizer::NCURSESVisualizer() {
    initCurses();
    last_frame_time = std::chrono::steady_clock::now();
}

NCURSESVisualizer::~NCURSESVisualizer() {
    deinitCurses();
}

void NCURSESVisualizer::setInputController(InputController* ic) {
    inputController = ic;
}

void NCURSESVisualizer::setGameController(IGameController* gc) {
    gameController = gc;
}

void NCURSESVisualizer::initCurses() {
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    nodelay(stdscr, TRUE); // getch non-blocking
    curs_set(0);
    start_color();
    use_default_colors();
    init_pair(1, COLOR_WHITE, COLOR_BLUE);    // header
    init_pair(2, COLOR_BLACK, COLOR_YELLOW);  // selected menu
    init_pair(3, COLOR_GREEN, -1);            // player gauge
    init_pair(4, COLOR_RED, -1);              // health
    init_pair(5, COLOR_CYAN, -1);             // info
    updateTermSize();
    clear();
    refresh();
}

void NCURSESVisualizer::deinitCurses() {
    endwin();
}

void NCURSESVisualizer::updateTermSize() {
    getmaxyx(stdscr, term_h, term_w);
}

int NCURSESVisualizer::fetchInput() {
    // 0 means nothing
    if (inputController) {
        char c = inputController->getInputChar();
        if (c != 0) return (int)c;
    }
    int ch = getch();
    if (ch == ERR) return 0;
    return ch;
}

void NCURSESVisualizer::display() {
    state = State::MainMenu;
    while (state != State::Exit) {
        updateTermSize();
        switch (state) {
            case State::MainMenu:
                loopMainMenu();
                break;
            case State::InGame:
                loopInGame();
                break;
            case State::LevelUpMenu:
                loopLevelUp();
                break;
            default:
                break;
        }
    }
}

void NCURSESVisualizer::loopMainMenu() {
    // Target FPS ~ 30
    const int frame_ms = 33;
    auto frame_start = std::chrono::steady_clock::now();

    drawMainMenu();

    int input = fetchInput();
    if (input) {
        switch (input) {
            case KEY_UP:
            case 'w':
                main_menu_selected = (main_menu_selected - 1 + (int)main_menu_items.size()) % (int)main_menu_items.size();
                break;
            case KEY_DOWN:
            case 's':
                main_menu_selected = (main_menu_selected + 1) % (int)main_menu_items.size();
                break;
            case '\n':
            case 'e':
            case KEY_ENTER:
                if (main_menu_selected == 0) { // New Game
                    if (gameController) gameController->startNewGame();
                    state = State::InGame;
                } else if (main_menu_selected == 1) { // Continue
                    if (gameController) gameController->startNewGame(); // your original used startNewGame for continue
                    state = State::InGame;
                } else if (main_menu_selected == 2) { // Exit
                    state = State::Exit;
                }
                break;
            case 'q':
            case 'Q':
                state = State::Exit;
                break;
            default:
                break;
        }
    }

    // frame limiter
    auto frame_end = std::chrono::steady_clock::now();
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(frame_end - frame_start).count();
    if (ms < frame_ms) std::this_thread::sleep_for(std::chrono::milliseconds(frame_ms - ms));
}


void NCURSESVisualizer::loopLevelUp() {
    const int frame_ms = 33;
    auto frame_start = std::chrono::steady_clock::now();

    drawLevelUpMenu();

    int input = fetchInput();
    if (input) {
        switch (input) {
            case KEY_UP:
            case 'w': case 'W':
                levelup_selected = (levelup_selected - 1 + 3) % 3; break;
            case KEY_DOWN:
            case 's': case 'S':
                levelup_selected = (levelup_selected + 1) % 3; break;
            case '\n': case 'e': case KEY_ENTER:
                if (gameController) {
                    char choice = '1' + levelup_selected;
                    gameController->playerLevelUp(choice);
                }
                state = State::InGame;
                break;
        }
    }

    // frame limiter
    auto frame_end = std::chrono::steady_clock::now();
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(frame_end - frame_start).count();
    if (ms < frame_ms) std::this_thread::sleep_for(std::chrono::milliseconds(frame_ms - ms));
}


void NCURSESVisualizer::drawLevelUpMenu() {
    clear();
    // Заголовок
    attron(COLOR_PAIR(1));
    mvhline(0, 0, ' ', term_w);
    mvprintw(0, (term_w - 15) / 2, " LEVEL UP! ");
    attroff(COLOR_PAIR(1));

    if (!gameController) return;

    auto pdata = gameController->getPlayerData();
    if (!pdata) return;

    int box_w = 50;
    int box_h = 10;
    int bx = (term_w - box_w) / 2;
    int by = (term_h - box_h) / 2;

    // Draw border
    for (int i = 0; i < box_w; ++i) { mvaddch(by, bx + i, '-'); mvaddch(by + box_h -1, bx + i, '-'); }
    for (int i = 0; i < box_h; ++i) { mvaddch(by + i, bx, '|'); mvaddch(by + i, bx + box_w - 1, '|'); }
    mvaddch(by, bx, '+'); mvaddch(by, bx + box_w -1, '+');
    mvaddch(by + box_h -1, bx, '+'); mvaddch(by + box_h -1, bx + box_w -1, '+');

    // Выбор атрибута
    std::vector<std::string> options = {
        "INT +10",
        "STR +10",
        "DEX +10"
    };

    for (size_t i = 0; i < options.size(); ++i) {
        int iy = by + 2 + (int)i;
        if ((int)i == levelup_selected) {
            attron(COLOR_PAIR(2));
            mvprintw(iy, bx + 5, " %s ", options[i].c_str());
            attroff(COLOR_PAIR(2));
        } else {
            mvprintw(iy, bx + 5, " %s ", options[i].c_str());
        }
    }

    // Подсказка
    std::string hint = "Use arrows/WASD + Enter to choose";
    mvprintw(by + box_h -2, bx + (box_w - (int)hint.size())/2, "%s", hint.c_str());

    // Полная инфо об игроке
    int info_y = by + box_h + 1;
    mvprintw(info_y++, bx, "Level: %d  Exp: %lld/%lld", pdata->playerLevel, pdata->playerCurrentExperience, pdata->playerLevelUpExperience);
    mvprintw(info_y++, bx, "Health: %d/%d  Attack: %d  Weapon: %s", pdata->playerHealth, pdata->playerMaxHealth, pdata->playerAttack, pdata->playerWeapon.c_str());
    mvprintw(info_y++, bx, "INT: %d  STR: %d  DEX: %d", pdata->playerIntelligence, pdata->playerStrength, pdata->playerDexterity);
    mvprintw(info_y++, bx, "Debuff: %s", pdata->playerDebaff.c_str());

    refresh();
}


void NCURSESVisualizer::drawMainMenu() {
    clear();
    // Header
    attron(COLOR_PAIR(1));
    mvhline(0, 0, ' ', term_w);
    mvprintw(0, (term_w - 12) / 2, " MAIN MENU ");
    attroff(COLOR_PAIR(1));

    // Menu box
    int box_w = 40;
    int box_h = (int)main_menu_items.size() + 4;
    int bx = (term_w - box_w) / 2;
    int by = (term_h - box_h) / 2;
    // Draw border
    for (int i = 0; i < box_w; ++i) mvaddch(by, bx + i, '-');
    for (int i = 0; i < box_w; ++i) mvaddch(by + box_h - 1, bx + i, '-');
    for (int i = 0; i < box_h; ++i) {
        mvaddch(by + i, bx, '|');
        mvaddch(by + i, bx + box_w - 1, '|');
    }
    mvaddch(by, bx, '+'); mvaddch(by, bx + box_w - 1, '+');
    mvaddch(by + box_h - 1, bx, '+'); mvaddch(by + box_h - 1, bx + box_w - 1, '+');

    // Title
    std::string title = "Choose action";
    mvprintw(by + 1, bx + (box_w - (int)title.size())/2, "%s", title.c_str());

    // Items
    for (size_t i = 0; i < main_menu_items.size(); ++i) {
        int iy = by + 2 + (int)i;
        if ((int)i == main_menu_selected) {
            attron(COLOR_PAIR(2));
            mvprintw(iy, bx + 3, " %s ", main_menu_items[i].c_str());
            attroff(COLOR_PAIR(2));
        } else {
            mvprintw(iy, bx + 3, " %s ", main_menu_items[i].c_str());
        }
    }

    // Footer
    std::string hint = "Use arrows/WASD + Enter. Q to quit.";
    mvprintw(by + box_h, bx + (box_w - (int)hint.size())/2, "%s", hint.c_str());

    refresh();
}

void NCURSESVisualizer::loopInGame() {
    // ~30 FPS
    const int frame_ms = 33;
    auto frame_start = std::chrono::steady_clock::now();

    drawInGame();

    int input = fetchInput();
    if (input) {
        // navigation / actions
        if (input == 27) { // ESC
            // stop game and go to main menu
            if (gameController) gameController->stopGame();
            state = State::MainMenu;
            return;
        }

        // movement keys + actions
        char c = (char)input;
        if (gameController) {
            bool consumed = false;
            // accept arrows as well
            if (input == KEY_UP || c == 'w' || c == 'W') { consumed = !gameController->performAnAction('w'); }
            else if (input == KEY_DOWN || c == 's' || c == 'S') { consumed = !gameController->performAnAction('s'); }
            else if (input == KEY_LEFT || c == 'a' || c == 'A') { consumed = !gameController->performAnAction('a'); }
            else if (input == KEY_RIGHT || c == 'd' || c == 'D') { consumed = !gameController->performAnAction('d'); }
            else if (c == 'e' || c == 'E') { consumed = !gameController->performAnAction('e'); }
            else if (c == 'q' || c == 'Q') { gameController->performAnAction('q'); }
            else if (c >= '1' && c <= '4') {
                // map '1'..'4' to '0'..'3' as in original
                gameController->performAnAction((char)('0' + (c - '1')));
            }
            // If performAnAction returned false -> game ended, return to menu
            // We checked consumed above to detect false return (inverted). Simpler: check separately:
            // But we already inverted above; to be safe, check gameController state by calling performAnAction and reacting if false:
            // (we already called it above; if it returned false we set state accordingly)
            if (consumed) {
                state = State::MainMenu;
                return;
            }
        }
    }

    // frame limiter
    auto frame_end = std::chrono::steady_clock::now();
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(frame_end - frame_start).count();
    if (ms < frame_ms) std::this_thread::sleep_for(std::chrono::milliseconds(frame_ms - ms));
}

void NCURSESVisualizer::drawInGame() {
    clear();
    // Header
    attron(COLOR_PAIR(1));
    mvhline(0, 0, ' ', term_w);
    mvprintw(0, (term_w - 12) / 2, " IN GAME ");
    attroff(COLOR_PAIR(1));

    // Layout: left (25%), center (50%), right (25%)
    int left_w = term_w / 4;
    int right_w = term_w / 4;
    int center_w = term_w - left_w - right_w - 2; // -2 for separators
    int top = 1;
    int height = term_h - 2;

    // Vertical separators
    for (int y = top; y <= top + height; ++y) {
        mvaddch(y, left_w, '|');
        mvaddch(y, left_w + 1 + center_w, '|');
    }

    drawLeftPanel(0, top, left_w, height);
    drawFieldPanel(left_w + 1, top, center_w, height);
    drawRightPanel(left_w + 1 + center_w + 1, top, right_w, height);

    // Footer / hint
    std::string hint = "WASD/arrows - move | Q/E - actions | Esc - menu | 1-4 - use item";
    mvprintw(term_h - 1, (term_w - (int)hint.size()) / 2, "%s", hint.c_str());

    refresh();
}

void NCURSESVisualizer::drawBoxTitle(int x, int y, int w, const std::string& title) {
    // title at top center of box area
    mvhline(y, x, ' ', w);
    mvprintw(y, x + (w - (int)title.size())/2, "%s", title.c_str());
}

void NCURSESVisualizer::drawLeftPanel(int x, int y, int w, int h) {
    // player info at top half, enemy info bottom half
    int half = h / 2;
    int cur_y = y;

    // Player info box
    drawBoxTitle(x, cur_y, w, " PLAYER INFO ");
    cur_y++;
    if (gameController) {
        auto data = gameController->getPlayerData();
        if (data) {
            // Health bar and basic stats
            float health = (data->playerMaxHealth > 0)
                ? (float)data->playerHealth / (float)data->playerMaxHealth : 0.0f;
            int bar_w = w - 4;
            int filled = (int)(bar_w * health);
            mvprintw(cur_y++, x + 1, "Health: %d/%d", data->playerHealth, data->playerMaxHealth);
            mvprintw(cur_y, x + 1, "[");
            for (int i = 0; i < bar_w; ++i) {
                if (i < filled) addch('=');
                else addch(' ');
            }
            addch(']');
            cur_y += 2;
            mvprintw(cur_y++, x + 1, "Level: %d", data->playerLevel);
            mvprintw(cur_y++, x + 1, "Exp: %lld/%lld", data->playerCurrentExperience, data->playerLevelUpExperience);
            mvprintw(cur_y++, x + 1, "Attack: %d", data->playerAttack);
            mvprintw(cur_y++, x + 1, "Weapon: %s", data->playerWeapon.c_str());
        } else {
            mvprintw(cur_y++, x + 1, "(no player data)");
        }
    } else {
        mvprintw(cur_y++, x + 1, "(no game controller)");
    }

    // Enemy info box
    cur_y = y + half;
    drawBoxTitle(x, cur_y, w, " ENEMIES ");
    cur_y++;
    if (gameController) {
        auto enemies = gameController->getEnemyData();
        if (!enemies.empty()) {
            for (size_t i = 0; i < enemies.size() && cur_y < y + h - 1; ++i) {
                mvprintw(cur_y++, x + 1, "%s %d/%d atk:%d",
                         enemies[i].name.c_str(),
                         enemies[i].enemyHealth,
                         enemies[i].enemyMaxHealth,
                         enemies[i].enemyAttack);
            }
        } else {
            mvprintw(cur_y++, x + 1, "(no enemies)");
        }
    } else {
        mvprintw(cur_y++, x + 1, "(no game controller)");
    }
}


void NCURSESVisualizer::drawFieldPanel(int x, int y, int w, int h) {
    drawBoxTitle(x, y, w, " FIELD ");

    std::vector<wchar_t> fieldChars;
    int fw = GlobalGameConfig::fieldWidth;
    int fh = GlobalGameConfig::fieldHeight;

    if (gameController)
        fieldChars = gameController->getFieldData();

    int render_cell_width = 2;
    int render_field_width = fw * render_cell_width;
    int render_field_height = fh;

    // ВИДИМАЯ область - вся панель минус отступы
    int visible_w = w - 2;  // минус границы
    int visible_h = h - 2;  // минус границы

    // ЦЕНТРИРОВАНИЕ: всегда центрируем, если поле меньше панели
    // Если поле больше панели - показываем центральную часть
    int start_x = x + 1;
    int start_y = y + 1;

    int offset_x = 0;
    int offset_y = 0;

    // Если поле меньше панели по ширине - центрируем
    if (render_field_width < visible_w) {
        start_x = x + 1 + (visible_w - render_field_width) / 2;
    } else {
        // Иначе обрезаем по бокам
        offset_x = (render_field_width - visible_w) / 2;
    }

    // Если поле меньше панели по высоте - центрируем
    if (render_field_height < visible_h) {
        start_y = y + 1 + (visible_h - render_field_height) / 2;
    } else {
        // Иначе обрезаем сверху/снизу
        offset_y = (render_field_height - visible_h) / 2;
    }

    // ВЫВОД с правильной индексацией
    for (int ry = 0; ry < std::min(visible_h, render_field_height); ++ry) {
        int actual_ry = ry + offset_y;  // Учитываем смещение для обрезания
        if (actual_ry >= fh) break;     // Выход за границы поля

        int base_idx = actual_ry * fw;  // Правильная индексация row-major

        for (int rx = 0; rx < std::min(visible_w / 2, fw); ++rx) {
            int actual_rx = rx + offset_x;  // Учитываем смещение для обрезания
            if (actual_rx >= fw) break;     // Выход за границы поля

            int idx = base_idx + actual_rx;  // Индекс в данных поля
            if (idx >= (int)fieldChars.size()) break;

            wchar_t ch = fieldChars[idx];
            char out = (ch > 0 && ch < 128) ? (char)ch : '.';

            // Позиция на экране
            int screen_x = start_x + rx * 2;
            int screen_y = start_y + ry;

            if (screen_x < x + w && screen_y < y + h) {
                mvaddch(screen_y, screen_x, out);
            }
        }
    }
}

void NCURSESVisualizer::drawRightPanel(int x, int y, int w, int h) {
    // Top: Hand / spells
    drawBoxTitle(x, y, w, " HAND ");
    int cur_y = y + 1;

    if (gameController) {
        auto pdata = gameController->getPlayerData();
        if (pdata->levelIncreased) {
            this->state = State::LevelUpMenu;
        }
        if (pdata) {
            for (int i = 0; i < pdata->playerCurrentHandSize && cur_y < y + h - 3; ++i) {
                auto item = pdata->playerHandItem[i];
                std::string sel = item.first ? "*" : " ";
                std::string spell_name;
                switch(i) {
                    case 0: spell_name = "AOE Spell"; break;
                    case 1: spell_name = "Fireball"; break;
                    case 2: spell_name = "Upgrade"; break;
                    case 3: spell_name = "Trap"; break;
                }
                mvprintw(cur_y++, x + 1, "%d) %s %s x%d", i+1, sel.c_str(), spell_name.c_str(), item.second);
            }
        }
    } else {
        mvprintw(cur_y++, x + 1, "(no controller)");
    }

    // Bottom: Save & Exit button
    std::string btn = "[ Save & Exit ]";
    mvprintw(y + h - 2, x + (w - (int)btn.size())/2, "%s", btn.c_str());
    mvprintw(y + h - 1, x + 1, "Press 'S' to save & exit");
}

// End of file
