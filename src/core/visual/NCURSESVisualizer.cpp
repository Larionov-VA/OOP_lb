#include <ncurses.h>
#include <iostream>
#include <cstring>
#include <unistd.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/rfcomm.h>
#include <atomic>
#include <thread>

std::atomic<bool> running{true};
int pin_states[6] = {0}; // 0=off, 1=on

void update_display() {
    clear();

    // Заголовок
    mvprintw(0, 0, "ESP32 Signal Monitor - PIN States");
    mvprintw(1, 0, "Press 'q' to quit");

    // Отображаем состояние пинов
    for(int i = 0; i < 6; i++) {
        int y = 3 + i * 2;
        int x = 10;

        mvprintw(y, 0, "PIN %d:", i+1);

        if(pin_states[i]) {
            attron(COLOR_PAIR(2)); // Зеленый
            mvprintw(y, x, "[ACTIVE]");
            attroff(COLOR_PAIR(2));
        } else {
            attron(COLOR_PAIR(1)); // Красный
            mvprintw(y, x, "[INACTIVE]");
            attroff(COLOR_PAIR(1));
        }
    }

    // Инструкции
    mvprintw(16, 0, "Connect GND to pins 15-21 on ESP32 to activate");
    mvprintw(17, 0, "Bluetooth device: ESP32-Signal");

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
            // Обрабатываем команды
            for(int i = 1; i <= 6; i++) {
                if(strstr(buffer, ("PIN" + std::to_string(i)).c_str())) {
                    pin_states[i-1] = 1;
                    // Автоматическое выключение через 2 секунды
                    std::thread([i]() {
                        usleep(2000000); // 2 секунды
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
    timeout(100); // Неблокирующий ввод

    // Инициализация цветов
    start_color();
    init_pair(1, COLOR_RED, COLOR_BLACK);    // Неактивный
    init_pair(2, COLOR_GREEN, COLOR_BLACK);  // Активный

    // Запуск Bluetooth потока
    std::thread bt_thread(bluetooth_thread, argv[1]);

    // Главный цикл отображения
    while(running) {
        update_display();

        // Проверка ввода
        int ch = getch();
        if(ch == 'q' || ch == 'Q') {
            running = false;
        }

        usleep(50000); // 50ms
    }

    // Завершение
    bt_thread.join();
    endwin();
    return 0;
}