#include <stdlib.h>

#include "2048.h"

int main() {
    (void)initscr();

    if (enable_colors && has_colors()) {
        do_initialize_colors();
    }

    srand(time(NULL));

    cbreak();
    noecho();
    keypad(stdscr, TRUE);

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
        wmove(stdscr, 0, 0);
        refresh();
        input = getch();
        switch (input) {
        case 'n':
            new_game();
            goto start;
        case 'q':
            goto end;
        case KEY_RIGHT:
            right(1);
            break;
        case KEY_DOWN:
            down(1);
            break;
        case KEY_LEFT:
            left(1);
            break;
        case KEY_UP:
            up(1);
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
