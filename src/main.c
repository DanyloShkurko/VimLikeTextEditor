#include <ncurses.h>
#include "win_manager/win_manager.h"
#include <stdlib.h>

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
            printf("h: %d, w: %d", h, w);

            int sd_h = 10;
            int sd_w = 100;

            int sd_start_y = (h-sd_h)/2;
            int sd_start_x = (w-sd_w)/2;

            WINDOW *save_dialog = create_newwindow(sd_h, sd_w, sd_start_y, sd_start_x);
            wrefresh(save_dialog);

            wmove(save_dialog, sd_h/2, (sd_w-70)/2);
            wprintw(save_dialog, "Do you want to save this file?\n\t\t\t [Y]es [N]o\n");
            
            wrefresh(save_dialog);

            refresh();
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