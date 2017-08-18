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
        input = getch();

        (void)flushinp();

        switch (input) {
        case 'n':
            new_game();
            goto start;
        case 'q':
            goto end;
        case KEY_RIGHT:
            board_move(right);
            break;
        case KEY_DOWN:
            board_move(down);
            break;
        case KEY_LEFT:
            board_move(left);
            break;
        case KEY_UP:
            board_move(up);
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
