#include "file_manager.h"

int load_file(WINDOW *window, WINDOW *num_bar, char *path, int *line)
{
    int ch, prev;
    FILE *fp;
    int goto_prev = FALSE, y, x;

    fp = fopen(path, "r");

    if (fp == NULL)
    {
        perror("\n\tCannot open input file!");
        return 1;
    }

    prev = EOF;
    while ((ch = fgetc(fp)) != EOF)
    {
        if (prev == '/' && ch == '*')
        {
            wattron(window, A_BOLD);
            goto_prev = TRUE;
        }

        if (goto_prev == TRUE)
        {
            getyx(window, y, x);
            wmove(window, y, x - 1);

            wprintw(window, "%c%c", '/', ch);

            ch = 'a';

            goto_prev = FALSE;
        }
        else
        {
            wprintw(window, "%c", ch);
            wrefresh(window);

            if (prev == '*' && ch == '/')
            {
                wattroff(window, A_BOLD);
            }
            prev = ch;
        }

        if (ch == '\n')
        {
            char *line_num = (char *)malloc(6 * sizeof(char));
            sprintf(line_num, "%d\n", (*line)++);

            waddstr(num_bar, line_num);
        }
    }

    return 0;
}