#include "file_manager.h"
#include <stdio.h>
#include <string.h>

#define LINE_BUF_SIZE 4096

int load_file(Buffer *buf, const char *path) {
  FILE *fp = fopen(path, "r");
  if (fp == NULL)
    return 1;

  char line_buf[LINE_BUF_SIZE];
  while (fgets(line_buf, sizeof(line_buf), fp)) {
    int len = (int)strlen(line_buf);
    if (len > 0 && line_buf[len - 1] == '\n')
      line_buf[len - 1] = '\0';
    buffer_insert_line(buf, buf->count, line_buf);
  }

  fclose(fp);
  return 0;
}

int save_file(const Buffer *buf, const char *path) {
  FILE *fp = fopen(path, "w");
  if (fp == NULL) {
    perror("\n\tCannot open output file!");
    return 1;
  }

  for (int i = 0; i < buf->count; i++) {
    fputs(buf->lines[i], fp);
    fputc('\n', fp);
  }

  fclose(fp);
  return 0;
}
