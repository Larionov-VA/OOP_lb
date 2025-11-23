#include <ncurses.h>
#include <iostream>
#include <cstring>
#include <unistd.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/rfcomm.h>
#include <atomic>
#include <thread>

std::atomic<bool> running{true};
int pin_states[6] = {0};

void update_display() {
    clear();

    mvprintw(0, 0, "ESP32 Signal Monitor - Controls: 1-6 (keyboard), Q (quit)");

    // Отображаем состояние пинов
    for(int i = 0; i < 6; i++) {
        int y = 2 + i;
        mvprintw(y, 0, "PIN %d:", i+1);

        if(pin_states[i]) {
            attron(COLOR_PAIR(2));
            mvprintw(y, 10, "[ACTIVE]");
            attroff(COLOR_PAIR(2));
        } else {
            attron(COLOR_PAIR(1));
            mvprintw(y, 10, "[INACTIVE]");
            attroff(COLOR_PAIR(1));
        }
    }

    mvprintw(10, 0, "Controls:");
    mvprintw(11, 0, "Keyboard: Press 1-6 to activate pins");
    mvprintw(12, 0, "Bluetooth: Connect ESP32 and use GND on pins 15-21");
    mvprintw(13, 0, "Press 'q' to quit");

    refresh();
}

void bluetooth_thread(const std::string& mac_addr) {
    int sock = socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM);
    if(sock < 0) return;

    struct sockaddr_rc addr = {0};
    addr.rc_family = AF_BLUETOOTH;
    addr.rc_channel = 1;
    str2ba(mac_addr.c_str(), &addr.rc_bdaddr);

    if(connect(sock, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        close(sock);
        return;
    }

    char buffer[256];
    while(running) {
        memset(buffer, 0, sizeof(buffer));
        int len = read(sock, buffer, sizeof(buffer)-1);

        if(len > 0) {
            for(int i = 1; i <= 6; i++) {
                if(strstr(buffer, ("PIN" + std::to_string(i)).c_str())) {
                    pin_states[i-1] = 1;
                    std::thread([i]() {
                        usleep(500000); // 0.5 секунды
                        pin_states[i-1] = 0;
                    }).detach();
                }
            }
        }
        else if(len == 0) break;

        usleep(100000);
    }
    close(sock);
}

int main(int argc, char* argv[]) {
    if(argc != 2) {
        std::cout << "Usage: " << argv[0] << " <MAC_address>" << std::endl;
        return 1;
    }

    // Инициализация ncurses
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    curs_set(0);
    timeout(100);

    start_color();
    init_pair(1, COLOR_RED, COLOR_BLACK);
    init_pair(2, COLOR_GREEN, COLOR_BLACK);

    // Запуск Bluetooth потока
    std::thread bt_thread(bluetooth_thread, argv[1]);

    // Главный цикл
    while(running) {
        update_display();

        int ch = getch();
        switch(ch) {
            case '1': case '2': case '3': case '4': case '5': case '6':
                {
                    int pin = ch - '1'; // Convert '1' to 0, '2' to 1, etc.
                    pin_states[pin] = 1;
                    std::thread([pin]() {
                        usleep(500000);
                        pin_states[pin] = 0;
                    }).detach();
                }
                break;
            case 'q': case 'Q':
                running = false;
                break;
        }

        usleep(50000);
    }

    bt_thread.join();
    endwin();
    return 0;
}