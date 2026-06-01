#include "buffer/buffer.h"
#include "changes/changes_manager.h"
#include "file_manager/file_manager.h"
#include "mode_handler/mode_handler.h"
#include <ncurses.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NORMAL 'N'
#define INSERT 'I'

void update_num_bar(WINDOW *num_bar, int start_line, int total_lines);

int main(int argc, char *argv[]) {
  init_changes_management();
  initscr();
  noecho();
  cbreak();
  change_cursor_mode(1);
  set_escdelay(25);

  int num_bar_w = 5;
  int parent_x, parent_y;
  int cursor_x = 0, cursor_y = 0;
  int view_top = 0;

  getmaxyx(stdscr, parent_y, parent_x);

  WINDOW *num_bar = newwin(parent_y - 1, num_bar_w, 0, 0);
  WINDOW *status_bar = newwin(1, parent_x, parent_y - 1, 0);
  WINDOW *main_win = newwin(parent_y - 1, parent_x - num_bar_w, 0, num_bar_w);

  keypad(main_win, TRUE);

  start_color();
  init_pair(1, COLOR_BLACK, COLOR_WHITE);
  wbkgd(status_bar, COLOR_PAIR(1));

  Buffer buf;
  buffer_init(&buf);

  char *path = NULL;
  if (argc > 1) {
    path = argv[1];
    if (load_file(&buf, path) == 1) {
      buffer_insert_line(&buf, 0, "");
      mvwprintw(status_bar, 0, 0, " New File: %s", path);
    } else {
      mvwprintw(status_bar, 0, 0, " Loaded: %s", path);
    }
  } else {
    buffer_insert_line(&buf, 0, "");
    mvwprintw(status_bar, 0, 0, " [No File]");
  }

  int win_height = getmaxy(main_win);

  buffer_render(&buf, main_win, view_top);
  update_num_bar(num_bar, 1, buf.count);
  wmove(main_win, cursor_y, cursor_x);
  wrefresh(main_win);
  wrefresh(status_bar);

  char prev_ch = 0;
  int ch;
  char mode = NORMAL;
  bool skip_prev_ch_update = false;

  while (1) {
    ch = wgetch(main_win);

    if (ch == 'q' && mode == NORMAL)
      break;

    if (mode == NORMAL) {
      handle_normal(status_bar, main_win, &skip_prev_ch_update, &buf, &mode,
                    &prev_ch, &ch, &cursor_x, &cursor_y, path);
    } else if (mode == INSERT) {
      handle_insert(status_bar, main_win, &mode, &buf, &ch, &cursor_x,
                    &cursor_y);
    }

    if (!skip_prev_ch_update)
      prev_ch = ch;

    view_top = cursor_y - win_height / 2;
    if (view_top < 0)
      view_top = 0;
    int max_view_top = buf.count > win_height ? buf.count - win_height : 0;
    if (view_top > max_view_top)
      view_top = max_view_top;

    buffer_render(&buf, main_win, view_top);
    update_num_bar(num_bar, view_top + 1, buf.count);
    wmove(main_win, cursor_y - view_top, cursor_x);
    wrefresh(main_win);

    skip_prev_ch_update = false;
  }

  endwin();
  buffer_free(&buf);
  clear_changes();

  return 0;
}

void update_num_bar(WINDOW *num_bar, int start_line, int total_lines) {
  werase(num_bar);

  int max_y, max_x;
  getmaxyx(num_bar, max_y, max_x);

  for (int i = 0; i < max_y; ++i) {
    int current_line = start_line + i;
    if (current_line <= total_lines)
      mvwprintw(num_bar, i, 0, "%-4d", current_line);
  }
  wrefresh(num_bar);
}
