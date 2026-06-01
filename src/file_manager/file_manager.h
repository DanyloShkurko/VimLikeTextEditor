#ifndef FILE_H
#define FILE_H

#include "../buffer/buffer.h"
#include <stdio.h>
#include <stdlib.h>

int load_file(Buffer *buf, const char *path);
int save_file(const Buffer *buf, const char *path);

#endif
