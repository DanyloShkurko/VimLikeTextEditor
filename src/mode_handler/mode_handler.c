#include "mode_handler.h"
#include "../changes/changes_manager.h"
#include "../file_manager/file_manager.h"
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NORMAL 'N'
#define INSERT 'I'

#define KEY_ESC 27
#define KEY_DELETE 127
#define CTRL_S 19
#define ENTER 10

static void insert_char_at(Buffer *buf, int y, int x, char c) {
  const char *line = buffer_get_line(buf, y);
  int len = (int)strlen(line);
  char *new_line = malloc(len + 2);
  memcpy(new_line, line, x);
  new_line[x] = c;
  memcpy(new_line + x + 1, line + x, len - x + 1);
  buffer_set_line(buf, y, new_line);
  free(new_line);
}

static void delete_char_before(Buffer *buf, int y, int x) {
  const char *line = buffer_get_line(buf, y);
  int len = (int)strlen(line);
  char *new_line = malloc(len);
  memcpy(new_line, line, x - 1);
  memcpy(new_line + x - 1, line + x, len - x + 1);
  buffer_set_line(buf, y, new_line);
  free(new_line);
}

void handle_normal(WINDOW *status_bar, WINDOW *win, bool *skip, Buffer *buf,
                   char *mode, char *prev_ch, int *ch, int *x, int *y,
                   char *path) {
  mvwprintw(status_bar, 0, 1, "NORMAL");
  wrefresh(status_bar);

  if (*ch == 'l') {
    int len = (int)strlen(buffer_get_line(buf, *y));
    if (*x < len - 1)
      *x += 1;
  }
  if (*ch == 'h') {
    if (*x > 0)
      *x -= 1;
  }
  if (*ch == 'j') {
    if (*y < buf->count - 1) {
      *y += 1;
      int len = (int)strlen(buffer_get_line(buf, *y));
      int max_x = len > 0 ? len - 1 : 0;
      if (*x > max_x)
        *x = max_x;
    }
  }
  if (*ch == 'k') {
    if (*y > 0) {
      *y -= 1;
      int len = (int)strlen(buffer_get_line(buf, *y));
      int max_x = len > 0 ? len - 1 : 0;
      if (*x > max_x)
        *x = max_x;
    }
  }

  if (*ch == 'o') {
    Change change;
    change.action = ACTION_INSERT;
    change.y_start = *y + 1;
    change.content = NULL;
    append_change(&change);

    buffer_insert_line(buf, *y + 1, "");
    *mode = INSERT;
    change_cursor_mode(2);
    *y += 1;
    *x = 0;
  }
  if (*ch == 'O') {
    Change change;
    change.action = ACTION_INSERT;
    change.y_start = *y;
    change.content = NULL;
    append_change(&change);

    buffer_insert_line(buf, *y, "");
    *mode = INSERT;
    change_cursor_mode(2);
    *x = 0;
  }

  if (*ch == 'd') {
    if (*prev_ch == 'd') {
      if (buf->count > 1) {
        Change change;
        change.action = ACTION_DELETE;
        change.y_start = *y;
        change.y_end = *x;
        change.content = strdup(buffer_get_line(buf, *y));
        append_change(&change);

        buffer_delete_line(buf, *y);
      }

      if (*y >= buf->count)
        *y = buf->count - 1;
      *x = 0;

      *prev_ch = ' ';
      *skip = true;
    }
  }

  if (*ch == '$') {
    int len = (int)strlen(buffer_get_line(buf, *y));
    *x = len > 0 ? len - 1 : 0;
  }
  if (*ch == '0') {
    *x = 0;
  }
  if (*ch == 'i') {
    *mode = INSERT;
    change_cursor_mode(2);
  }
  if (*ch == 'a') {
    *mode = INSERT;
    int len = (int)strlen(buffer_get_line(buf, *y));
    if (*x < len)
      *x += 1;
    change_cursor_mode(2);
  }

  if (*ch == 'u') {
    Change last = remove_last_change();

    if (last.action == ACTION_INSERT || last.content != NULL) {
      switch (last.action) {
      case ACTION_EDIT:
        buffer_set_line(buf, last.y_start, last.content);
        *y = last.y_start;
        *x = last.y_end;
        break;

      case ACTION_DELETE:
        buffer_insert_line(buf, last.y_start, last.content);
        *y = last.y_start;
        *x = last.y_end;
        break;

      case ACTION_INSERT:
        buffer_delete_line(buf, last.y_start);
        *y = (last.y_start > 0) ? last.y_start - 1 : 0;
        *x = 0;
        break;

      case ACTION_MERGE: {
        char *newline_pos = strchr(last.content, '\n');
        if (newline_pos != NULL) {
          *newline_pos = '\0';
          char *line_above = last.content;
          char *line_deleted = newline_pos + 1;

          buffer_set_line(buf, last.y_start, line_above);
          buffer_insert_line(buf, last.y_end, line_deleted);
          *y = last.y_end;
          *x = 0;
        }
        break;
      }

      case ACTION_SPLIT: {
        buffer_set_line(buf, last.y_start, last.content);
        buffer_delete_line(buf, last.y_end);
        *y = last.y_start;
        *x = (int)strlen(last.content);
        break;
      }
      }

      if (last.content != NULL)
        free(last.content);
    }
  }

  if (*ch == ':') {
    while ((*ch = wgetch(win)) != KEY_ESC) {
      if (*ch == 'w') {
        while ((*ch = wgetch(win)) != ENTER) {
        }
        save_file(buf, path);
        break;
      }
    }
    *skip = true;
    *prev_ch = ' ';
  }
}

void handle_insert(WINDOW *status_bar, WINDOW *win, char *mode, Buffer *buf,
                   int *ch, int *x, int *y) {
  mvwprintw(status_bar, 0, 1, "INSERT");
  wrefresh(status_bar);

  if (*ch == KEY_ESC) {
    *mode = NORMAL;
    change_cursor_mode(1);
    int len = (int)strlen(buffer_get_line(buf, *y));
    if (*x >= len && len > 0)
      *x = len - 1;
    if (*x < 0)
      *x = 0;
  } else if (*ch == KEY_DELETE) {
    if (*x == 0 && *y > 0) {
      int y_above = *y - 1;

      Change change;
      change.action = ACTION_MERGE;
      change.y_start = y_above;
      change.y_end = *y;

      const char *above = buffer_get_line(buf, y_above);
      const char *curr = buffer_get_line(buf, *y);
      int len_above = (int)strlen(above);
      int len_curr = (int)strlen(curr);
      change.content = malloc(len_above + len_curr + 2);
      sprintf(change.content, "%s\n%s", above, curr);
      append_change(&change);

      int new_x = len_above;
      char *merged = malloc(len_above + len_curr + 1);
      memcpy(merged, above, len_above);
      memcpy(merged + len_above, curr, len_curr + 1);
      buffer_set_line(buf, y_above, merged);
      free(merged);
      buffer_delete_line(buf, *y);

      *y = y_above;
      *x = new_x;
    } else if (*x == 0 && *y == 0) {
    } else {
      Change change;
      change.action = ACTION_EDIT;
      change.y_start = *y;
      change.y_end = *x;
      change.content = strdup(buffer_get_line(buf, *y));
      append_change(&change);

      delete_char_before(buf, *y, *x);
      *x -= 1;
    }
  } else if (*ch == KEY_UP) {
    if (*y > 0) {
      *y -= 1;
      int len = (int)strlen(buffer_get_line(buf, *y));
      if (*x > len)
        *x = len;
    }
  } else if (*ch == KEY_DOWN) {
    if (*y < buf->count - 1) {
      *y += 1;
      int len = (int)strlen(buffer_get_line(buf, *y));
      if (*x > len)
        *x = len;
    }
  } else if (*ch == KEY_LEFT) {
    if (*x > 0)
      *x -= 1;
  } else if (*ch == KEY_RIGHT) {
    int len = (int)strlen(buffer_get_line(buf, *y));
    if (*x < len)
      *x += 1;
  } else if (*ch == ENTER) {
    Change change;
    change.action = ACTION_SPLIT;
    change.y_start = *y;
    change.y_end = *y + 1;
    change.content = strdup(buffer_get_line(buf, *y));
    append_change(&change);

    const char *line = buffer_get_line(buf, *y);
    char *top = malloc(*x + 1);
    memcpy(top, line, *x);
    top[*x] = '\0';
    char *bottom = strdup(line + *x);

    buffer_set_line(buf, *y, top);
    free(top);
    buffer_insert_line(buf, *y + 1, bottom);
    free(bottom);

    *y += 1;
    *x = 0;
  } else if (isprint(*ch)) {
    Change change;
    change.action = ACTION_EDIT;
    change.y_start = *y;
    change.y_end = *x;
    change.content = strdup(buffer_get_line(buf, *y));
    append_change(&change);

    insert_char_at(buf, *y, *x, (char)*ch);
    *x += 1;
  }
}

void change_cursor_mode(int mode) {
  fflush(stdout);
  if (mode == 1) {
    printf("\e[0 q");
    fflush(stdout);
  } else if (mode == 2) {
    printf("\e[5 q");
    fflush(stdout);
  }
}
