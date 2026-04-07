#include <ncurses.h>
#include "win_manager/win_manager.h"
#include <stdlib.h>
#include <string.h>

#define KEY_ESC 27
#define KEY_DELETE 127
#define CTRL_S 19

int main(int argc, char *argv[])
{
    int curx = 0, cury = 0, h, w;
    int ch;
    initscr();
    raw();

    getmaxyx(stdscr, h, w);

    noecho();

    keypad(stdscr, true);

    printw("Press ESC to exit");
    refresh();

    while ((ch = getch()) != KEY_ESC)
    {
        switch (ch)
        {
        case KEY_LEFT:
            getyx(stdscr, cury, curx);
            move(cury, curx - 1);

            refresh();

            break;

        case KEY_RIGHT:
            getyx(stdscr, cury, curx);
            move(cury, curx + 1);

            refresh();

            break;

        case KEY_UP:
            getyx(stdscr, cury, curx);
            move(cury - 1, curx);

            refresh();
            break;

        case KEY_DOWN:
            getyx(stdscr, cury, curx);
            move(cury + 1, curx);

            refresh();
            break;

        case KEY_DELETE:
            getyx(stdscr, cury, curx);
            delch();

            move(cury, curx - 1);
            refresh();
            break;

        case CTRL_S:

            int sd_h = 8;
            int sd_w = 50;

            int sd_start_y = (h - sd_h) / 2;
            int sd_start_x = (w - sd_w) / 2;

            WINDOW *save_dialog = create_newwindow(sd_h, sd_w, sd_start_y, sd_start_x);

            char *line1 = "Do you want to save this file?";
            char *line2 = "[Y]es\t[N]o";

            long line1_x = (sd_w - strlen(line1)) / 2;
            long line2_x = (sd_w - strlen(line2)) / 2;

            mvwprintw(save_dialog, (sd_h / 2) - 1, line1_x, "%s", line1);
            mvwprintw(save_dialog, (sd_h / 2) + 1, line2_x, "%s", line2);

            wrefresh(save_dialog);

            break;

        default:
            getyx(stdscr, cury, curx);
            addch(ch);

            move(cury, curx + 1);
            refresh();
        }
    }
    endwin();
    return 0;
}