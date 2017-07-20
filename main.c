#include <stdlib.h>
#include <time.h>

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

    int last = 0;
    int input = 0;

    if (!load_game()) {
        new_game();
    }

 start:
    while (1) {
        if (change) {
            insert_random_tile();
            change = 0;
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
        }
        if (input == last) {
            switch (input) {
            case KEY_RIGHT:
                right();
                break;
            case KEY_DOWN:
                down();
                break;
            case KEY_LEFT:
                left();
                break;
            case KEY_UP:
                up();
                break;
            }
            last = 0;
        } else {
            last = input;
        }
    }
 end:
    save_game();

    del_tiles();

    endwin();
    return EXIT_SUCCESS;
}
