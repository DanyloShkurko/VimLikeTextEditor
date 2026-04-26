#ifndef FILE_H
#define FILE_H

#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>

int load_file(WINDOW *window, WINDOW *num_bar, char *path, int *line);

#endif