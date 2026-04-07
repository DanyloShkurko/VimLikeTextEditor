#include <ncurses.h>

#define KEY_ESC 27

void load_file(char *path)
{
    int ch, prev;
    FILE *fp;
    int goto_prev = FALSE, y, x;

    fp = fopen(path, "r");

    if (fp == NULL)
    {
        perror("Cannot open input file!");
        exit(1);
    }

    initscr();

    prev = EOF;
    while ((ch = fgetc(fp)) != EOF)
    {
        if (prev == '/' && ch == '*')
        {
            attron(A_BOLD);
            goto_prev = TRUE;
        }

        if (goto_prev == TRUE)
        {
            getyx(stdscr, y, x);
            move(y, x - 1);

            printw("%c%c", '/', ch);

            ch = 'a';

            goto_prev = FALSE;
        }
        else
        {
            printw("%c", ch);
            refresh();

            if (prev == '*' && ch == '/')
            {
                attroff(A_BOLD);
            }
            prev = ch;
        }
    }
}