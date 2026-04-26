#include <ncurses.h>
#include "file_manager/file_manager.h"
#include <stdlib.h>
#include <string.h>

#define KEY_ESC 27
#define KEY_DELETE 127
#define CTRL_S 19

#define N "NORMAL"
#define I "INSERT"
#define V "VIEW"

int main(int argc, char *argv[])
{

    initscr();
    noecho();
    cbreak();

    int num_bar_w = 5;
    int parent_x, parent_y;
    int cursor_x = 0, cursor_y = 0;
    int last_line = 1;

    getmaxyx(stdscr, parent_y, parent_x);

    WINDOW *num_bar = newwin(parent_y-1, num_bar_w, 0, 0);
    WINDOW *status_bar = newwin(1, parent_x, parent_y - 1, 0);
    WINDOW *main_win = newwin(parent_y - 1, parent_x-num_bar_w, 0, num_bar_w);

    keypad(main_win, TRUE);

    start_color();
    init_pair(1, COLOR_BLACK, COLOR_WHITE);

    wbkgd(status_bar, COLOR_PAIR(1));
    mvwprintw(status_bar, 0, 1, " STATUS: READ-ONLY MODE | Press 'q' to quit");

    wmove(main_win, cursor_y, cursor_x);

    char *path = argv[1];
    if(load_file(main_win, num_bar, path, &last_line) == 1) {
        return 1;
    }

    wrefresh(main_win);
    wrefresh(status_bar);
    wrefresh(num_bar);

    int ch;

    while ((ch = wgetch(main_win)) != 'q')
    {
        getyx(main_win, cursor_y, cursor_x);
        if (ch == 'l') {
            wmove(main_win, cursor_y, cursor_x + 1);
        }
        if (ch == 'h') {
            wmove(main_win, cursor_y, cursor_x - 1);
        }
        if (ch == 'j') {
            wmove(main_win, cursor_y + 1, cursor_x);
        }
        if (ch == 'k') {
            wmove(main_win, cursor_y - 1, cursor_x);
        }

        wrefresh(main_win);
    }

    endwin();
    return 0;
}