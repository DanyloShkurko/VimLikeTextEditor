#include "win_manager.h"

WINDOW *create_newwindow(int h, int w, int starty, int startx) {
    WINDOW *local_win = newwin(h, w, starty, startx);
    box(local_win, 0 , 0);
    wborder(local_win, '|', '|', '-', '-', '+', '+', '+', '+');

    wrefresh(local_win);
    return local_win;
}

void destroy_window(WINDOW *window) {
    wborder(window, '|', '|', '-', '-', '+', '+', '+', '+');
    wrefresh(window);

    delwin(window);
}