#include <stdlib.h>
#include <locale.h>

#include "2048.h"

int main() {
    (void)setlocale(LC_ALL, "");
    (void)initscr();

    if (enable_colors && has_colors()) {
        do_initialize_colors();
    }

    srand(time(NULL));

    cbreak();
    noecho();
    nonl();
    intrflush(stdscr, FALSE);
    keypad(stdscr, TRUE);
    curs_set(0);

    init_tiles();

    int input = 0;

    if (!load_game()) {
        new_game();
    }

 start:
    while (1) {
        if (change) {
            change = 0;
            insert_random_tile();
        } else {
            int count = refresh_free_tiles();
            if (count == 0) {
                is_game_over();
            }
        }

        print_board();
        refresh();
        input = getch();

        (void)flushinp();

        switch (input) {
        case 'n':
            new_game();
            goto start;
        case 'q':
            goto end;
        case KEY_RIGHT:
            right();
            merge_right();
            right();
            break;
        case KEY_DOWN:
            down();
            merge_down();
            down();
            break;
        case KEY_LEFT:
            left();
            merge_left();
            left();
            break;
        case KEY_UP:
            up();
            merge_up();
            up();
            break;
        }
        input = 0;
        nanosleep(&sleep, NULL);
    }
 end:
    save_game();

    del_tiles();

    endwin();
    return EXIT_SUCCESS;
}
