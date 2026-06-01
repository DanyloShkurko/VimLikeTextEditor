#ifndef BUFFER_H
#define BUFFER_H

#include <ncurses.h>

typedef struct {
  char **lines;
  int count;
  int capacity;
} Buffer;

void buffer_init(Buffer *buf);
void buffer_free(Buffer *buf);
void buffer_insert_line(Buffer *buf, int y, const char *content);
void buffer_delete_line(Buffer *buf, int y);
void buffer_set_line(Buffer *buf, int y, const char *content);
const char *buffer_get_line(const Buffer *buf, int y);
void buffer_render(const Buffer *buf, WINDOW *win, int view_top);

#endif
