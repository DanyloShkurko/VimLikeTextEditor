#include "num_bar_manager.h"
#include <stdio.h>
#include <stdlib.h>

void append_line(WINDOW *num_bar, int *last_line) {
  (*last_line)++;

  char line_num[16];

  sprintf(line_num, "%d", *last_line);

  mvwprintw(num_bar, (*last_line) - 1, 0, line_num);
  wrefresh(num_bar);
}

void remove_line(WINDOW *main_win, WINDOW *num_bar, int *line) {
  int y, x;
  getyx(main_win, y, x);
  wmove(num_bar, *line - 1, 0);
  wmove(main_win, y, x);

  wdeleteln(num_bar);
  wdeleteln(main_win);
  wrefresh(num_bar);

  (*line)--;
}
