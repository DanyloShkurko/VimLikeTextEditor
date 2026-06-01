#ifndef MODE_HANDLER_H
#define MODE_HANDLER_H

#include "../buffer/buffer.h"
#include <stdbool.h>

void change_cursor_mode(int mode);

void handle_normal(WINDOW *status_bar, WINDOW *win, bool *skip, Buffer *buf,
                   char *mode, char *prev_ch, int *ch, int *x, int *y,
                   char *path);

void handle_insert(WINDOW *status_bar, WINDOW *win, char *mode, Buffer *buf,
                   int *ch, int *x, int *y);

#endif
