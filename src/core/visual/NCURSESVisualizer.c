#include <ncurses.h>
#include <stdlib.h>

int main() {
    int ch;
    int x = 0, y = 0;

    // Инициализация ncurses
    initscr();
    raw();              // Отключаем буферизацию ввода
    keypad(stdscr, TRUE); // Включаем специальные клавиши
    noecho();           // Не отображаем вводимые символы
    curs_set(0);        // Скрываем курсор

    // Получаем размеры экрана
    int max_y, max_x;
    getmaxyx(stdscr, max_y, max_x);

    // Начальная позиция в центре экрана
    x = max_x / 2;
    y = max_y / 2;

    printw("Use WASD to move, Q/E to exit. Press any key to start...");
    refresh();
    getch();
    clear();

    while(1) {
        // Очищаем экран
        clear();

        // Выводим инструкции
        mvprintw(0, 0, "Use WASD to move, Q/E to exit");
        mvprintw(1, 0, "Position: X=%d, Y=%d", x, y);

        // Рисуем символ в текущей позиции
        mvaddch(y, x, '@');

        // Обновляем экран
        refresh();

        // Получаем ввод
        ch = getch();

        // Обрабатываем клавиши
        switch(ch) {
            case 'w':
            case 'W':
                if (y > 0) y--;
                break;
            case 's':
            case 'S':
                if (y < max_y - 1) y++;
                break;
            case 'a':
            case 'A':
                if (x > 0) x--;
                break;
            case 'd':
            case 'D':
                if (x < max_x - 1) x++;
                break;
            case 'q':
            case 'Q':
            case 'e':
            case 'E':
                goto end;
                break;
        }
    }

end:
    // Завершаем работу с ncurses
    endwin();
    return 0;
}