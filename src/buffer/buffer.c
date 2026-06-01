#include "buffer.h"
#include <ncurses.h>
#include <stdlib.h>
#include <string.h>

#define INITIAL_CAPACITY 64

void buffer_init(Buffer *buf) {
  buf->capacity = INITIAL_CAPACITY;
  buf->count = 0;
  buf->lines = malloc(buf->capacity * sizeof(char *));
}

void buffer_free(Buffer *buf) {
  for (int i = 0; i < buf->count; i++)
    free(buf->lines[i]);
  free(buf->lines);
  buf->lines = NULL;
  buf->count = 0;
  buf->capacity = 0;
}

static void ensure_capacity(Buffer *buf) {
  if (buf->count >= buf->capacity) {
    buf->capacity *= 2;
    buf->lines = realloc(buf->lines, buf->capacity * sizeof(char *));
  }
}

void buffer_insert_line(Buffer *buf, int y, const char *content) {
  ensure_capacity(buf);
  memmove(&buf->lines[y + 1], &buf->lines[y],
          (buf->count - y) * sizeof(char *));
  buf->lines[y] = strdup(content ? content : "");
  buf->count++;
}

void buffer_delete_line(Buffer *buf, int y) {
  free(buf->lines[y]);
  memmove(&buf->lines[y], &buf->lines[y + 1],
          (buf->count - y - 1) * sizeof(char *));
  buf->count--;
}

void buffer_set_line(Buffer *buf, int y, const char *content) {
  free(buf->lines[y]);
  buf->lines[y] = strdup(content ? content : "");
}

const char *buffer_get_line(const Buffer *buf, int y) {
  if (y < 0 || y >= buf->count)
    return "";
  return buf->lines[y];
}

void buffer_render(const Buffer *buf, WINDOW *win, int view_top) {
  werase(win);
  int height = getmaxy(win);
  for (int i = 0; i < height; i++) {
    int line_idx = view_top + i;
    if (line_idx < buf->count)
      mvwaddstr(win, i, 0, buf->lines[line_idx]);
  }
}
