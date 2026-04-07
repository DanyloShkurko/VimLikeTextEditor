#ifndef WIN_MANAGER_H
#define WIN_MANAGER_H

#include <ncurses.h>

WINDOW *create_newwindow(int h, int w, int starty, int startx); 
void destroy_window(WINDOW *window);

#endif