// #include "NCURSESVisualizer.hpp"

// NCURSESVisualizer::NCURSESVisualizer() {
//     initscr();
//     cbreak();
//     noecho();
//     curs_set(0);
//     timeout(100);
//     controller_ = nullptr;
// }

// void NCURSESVisualizer::setController(IGameController* controller) {
//     controller_ = controller;
// }

// void NCURSESVisualizer::display() {
//     while (true)
//     {
//         clear();
//         mvprintw(0, 0, "NCURSES Visualizer - Working!");
//         refresh();
//         napms(1000); // Задержка 1 секунда чтобы увидеть сообщение
//     }
// }

// NCURSESVisualizer::~NCURSESVisualizer() {
//     endwin();
// }
#include "NCURSESVisualizer.hpp"
#include <locale.h>  // для setlocale

static bool ncurses_initialized = false;

NCURSESVisualizer::NCURSESVisualizer() {
    if (!ncurses_initialized) {
        // Устанавливаем локаль для поддержки wide characters
        setlocale(LC_ALL, "");

        initscr();
        cbreak();
        noecho();
        keypad(stdscr, TRUE);
        curs_set(0);
        timeout(100);

        // Включаем поддержку wide characters в ncurses
        if (has_colors()) {
            start_color();
        }

        ncurses_initialized = true;
    }
    controller_ = nullptr;
}

void NCURSESVisualizer::setController(IGameController* controller) {
    controller_ = controller;
}

void NCURSESVisualizer::display() {
    if (controller_ == nullptr) {
        mvprintw(0, 0, "Controller is NULL!");
        refresh();
        napms(2000);
        return;
    }

    controller_->startNewGame();

    while (true) {
        clear();

        try {
            std::vector<wchar_t> fieldChars = controller_->getFieldData();
            int expectedSize = GlobalGameConfig::fieldHeight * GlobalGameConfig::fieldWidth;

            if (fieldChars.size() >= static_cast<size_t>(expectedSize)) {
                for (int y = 0; y < GlobalGameConfig::fieldHeight; ++y) {
                    std::wstring line;
                    for (int x = 0; x < GlobalGameConfig::fieldWidth; ++x) {
                        int index = y * GlobalGameConfig::fieldWidth + x;
                        wchar_t symbol = (index < (int)fieldChars.size()) ? fieldChars[index] : L' ';
                        line += symbol;

                        // ИСПРАВЛЕННАЯ логика: добавляем пробел вместо нулевого символа
                        wchar_t nextSymbol = L' ';
                        if (symbol == L'🌳' || symbol == L'🌲' || symbol == L'🔥' ||
                            symbol == L'🏰' || symbol == L'⚔️' || symbol == L'🏟' ||
                            symbol == L'⛫' || symbol == L'𐇐' || symbol == L'𖨆' ||
                            symbol == L'⛰' || symbol == L'🏔') {
                            nextSymbol = L' ';  // ПРОБЕЛ вместо \0
                        }
                        else if (symbol == L'░') {
                            nextSymbol = L'░';  // для этого символа дублируем
                        }
                        // Для остальных символов nextSymbol остается пробелом

                        line += nextSymbol;
                    }
                    mvaddwstr(y, 0, line.c_str());
                }


                mvprintw(GlobalGameConfig::fieldHeight + 1, 0,
                        "Controls: WASD - move, Q - quit");
            }

            refresh();

            int ch = getch();
            if (ch == 'q' || ch == 'Q') {
                controller_->stopGame();
                clear();
                break;
            }
            else if (ch == 'w' || ch == 'W') controller_->performAnAction('w');
            else if (ch == 's' || ch == 'S') controller_->performAnAction('s');
            else if (ch == 'a' || ch == 'A') controller_->performAnAction('a');
            else if (ch == 'd' || ch == 'D') controller_->performAnAction('d');
            else if (ch == 'e' || ch == 'E') controller_->performAnAction('e');

            napms(50);

        } catch (...) {
            break;
        }
    }
}

NCURSESVisualizer::~NCURSESVisualizer() {
    if (ncurses_initialized) {
        endwin();
        ncurses_initialized = false;
    }
}


// void update_display() {
//     clear();

//     mvprintw(0, 0, "ESP32 Signal Monitor - Controls: 1-6 (keyboard), Q (quit)");

//     // Отображаем состояние пинов
//     for(int i = 0; i < 6; i++) {
//         int y = 2 + i;
//         mvprintw(y, 0, "PIN %d:", i+1);

//         if(pin_states[i]) {
//             attron(COLOR_PAIR(2));
//             mvprintw(y, 10, "[ACTIVE]");
//             attroff(COLOR_PAIR(2));
//         } else {
//             attron(COLOR_PAIR(1));
//             mvprintw(y, 10, "[INACTIVE]");
//             attroff(COLOR_PAIR(1));
//         }
//     }

//     mvprintw(10, 0, "Controls:");
//     mvprintw(11, 0, "Keyboard: Press 1-6 to activate pins");
//     mvprintw(12, 0, "Bluetooth: Connect ESP32 and use GND on pins 15-21");
//     mvprintw(13, 0, "Press 'q' to quit");

//     refresh();
// }

// void bluetooth_thread(const std::string& mac_addr) {
//     int sock = socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM);
//     if(sock < 0) return;

//     struct sockaddr_rc addr = {0};
//     addr.rc_family = AF_BLUETOOTH;
//     addr.rc_channel = 1;
//     str2ba(mac_addr.c_str(), &addr.rc_bdaddr);

//     if(connect(sock, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
//         close(sock);
//         return;
//     }

//     char buffer[256];
//     while(running) {
//         memset(buffer, 0, sizeof(buffer));
//         int len = read(sock, buffer, sizeof(buffer)-1);

//         if(len > 0) {
//             for(int i = 1; i <= 6; i++) {
//                 if(strstr(buffer, ("PIN" + std::to_string(i)).c_str())) {
//                     pin_states[i-1] = 1;
//                     std::thread([i]() {
//                         usleep(500000);
//                         pin_states[i-1] = 0;
//                     }).detach();
//                 }
//             }
//         }
//         else if(len == 0) break;

//         usleep(100000);
//     }
//     close(sock);
// }

// int main(int argc, char* argv[]) {
//     if(argc != 2) {
//         std::cout << "Usage: " << argv[0] << " <MAC_address>" << std::endl;
//         return 1;
//     }

//     // Инициализация ncurses
//     initscr();
//     cbreak();
//     noecho();
//     keypad(stdscr, TRUE);
//     curs_set(0);
//     timeout(100);

//     start_color();
//     init_pair(1, COLOR_RED, COLOR_BLACK);
//     init_pair(2, COLOR_GREEN, COLOR_BLACK);

//     // Запуск Bluetooth потока
//     std::thread bt_thread(bluetooth_thread, argv[1]);

//     // Главный цикл
//     while(running) {
//         update_display();

//         int ch = getch();
//         switch(ch) {
//             case '1': case '2': case '3': case '4': case '5': case '6':
//                 {
//                     int pin = ch - '1'; // Convert '1' to 0, '2' to 1, etc.
//                     pin_states[pin] = 1;
//                     std::thread([pin]() {
//                         usleep(500000);
//                         pin_states[pin] = 0;
//                     }).detach();
//                 }
//                 break;
//             case 'q': case 'Q':
//                 running = false;
//                 break;
//         }

//         usleep(50000);
//     }

//     bt_thread.join();
//     endwin();
//     return 0;
// }