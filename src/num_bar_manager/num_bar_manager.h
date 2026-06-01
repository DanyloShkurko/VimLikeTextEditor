#ifndef NUM_BAR_MANAGER_H
#define NUM_BAR_MANAGER_H

#include <ncurses.h>

void append_line(WINDOW *num_bar, int *last_line);
void remove_line(WINDOW *main_win, WINDOW *num_bar, int *line);

#endif