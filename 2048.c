#include <stdlib.h>
#include <ncurses.h>
#include <time.h>
#include <string.h>

#define BOARD_SZ 4
const int BOARD_SIZE = BOARD_SZ;

char title[] = "2048";

unsigned int score = 0;
unsigned int change = 1;
unsigned int high_score = 0;

char* save_file_name = "save.game";

char* relation[] = {
    ".",
    "2",
    "4",
    "8",
    "16",
    "32",
    "64",
    "128",
    "256",
    "512",
    "1024",
    "2048",
    "4096",
    "8192",
    "16384",
    "32768",
    "65536",
    "131072",
    "262144",
    "524288",
    "1048576",
    "2097152",
    "4194304"
};

char board[BOARD_SZ*BOARD_SZ] = {0};
int free_tiles[BOARD_SZ*BOARD_SZ];

void save_game() {
    FILE* fp = fopen(save_file_name, "wb");
    if (fp) {
        if (fwrite(&BOARD_SIZE, sizeof(BOARD_SIZE), 1, fp) != 1) {
            fputs("Could not write the board size to the save game file\n", stderr);
            exit(EXIT_FAILURE);
        }
        if (fwrite(&score, sizeof(score), 1, fp) != 1) {
            fputs("Could not write the score to the save game file\n", stderr);
            exit(EXIT_FAILURE);
        }
        if (fwrite(&high_score, sizeof(high_score), 1, fp) != 1) {
            fputs("Could not write the high score to the save game file\n", stderr);
            exit(EXIT_FAILURE);
        }
        if (fwrite(board, sizeof(*board), BOARD_SIZE*BOARD_SIZE, fp) != BOARD_SZ*BOARD_SZ) {
            fputs("Could not write the board data to the save game file\n", stderr);
            exit(EXIT_FAILURE);
        }
    }
}

int load_game() {
    int loaded = 0;
    FILE* fp = fopen(save_file_name, "rb");
    if (fp) {
        int board_size = 0;
        if (fread(&board_size, sizeof(board_size), 1, fp) != 1) {
            fputs("Could not read the board size from the save game file\n", stderr);
            exit(EXIT_FAILURE);
        } else {
            if (board_size != BOARD_SIZE) {
                fputs("Save game board size and actual board size do not match\n", stderr);                
                exit(EXIT_FAILURE);
            }
        }
        if (fread(&score, sizeof(score), 1, fp) != 1) {
            fputs("Could not read the score from the save game file\n", stderr);
            exit(EXIT_FAILURE);
        }
        if (fread(&high_score, sizeof(high_score), 1, fp) != 1) {
            fputs("Could not read the high score from the save game file\n", stderr);
            exit(EXIT_FAILURE);
        }
        if (fread(board, sizeof(*board), BOARD_SIZE*BOARD_SIZE, fp) != BOARD_SZ*BOARD_SZ) {
            fputs("Could not read the board data from the save game file\n", stderr);
            exit(EXIT_FAILURE);
        }
        loaded = 1;
    }
    return loaded;
}

WINDOW* tiles[BOARD_SZ][BOARD_SZ];

#define TILE_SIZE 9

void init_tiles(void) {
    int y, x;
    for (y = 0; y < BOARD_SIZE; ++y) {
        for (x = 0; x < BOARD_SIZE; ++x) {
            tiles[y][x] = subwin(stdscr,
                                 TILE_SIZE/2, TILE_SIZE,
                                 1 + y*(TILE_SIZE/2), x*TILE_SIZE);
        }
    }
}

void del_tiles(void) {
    int y, x;
    for (y = 0; y < BOARD_SIZE; ++y) {
        for (x = 0; x < BOARD_SIZE; ++x) {
            delwin(tiles[y][x]);
        }
    }    
}

void print_board(void) {
    int y, x;
    int rev = 0;
    (void)mvwaddstr(stdscr, 0, 1, "2048");
    (void)mvwprintw(stdscr, 0, 2*TILE_SIZE, "score: %d", score);
    for (y = 0; y < BOARD_SIZE; ++y) {
        for (x = 0; x < BOARD_SIZE; ++x) {
            wbkgd(tiles[y][x], ' ' | ((rev)?A_REVERSE:0));
            mvwprintw(tiles[y][x], TILE_SIZE/4, 1, "%7s", relation[(size_t)board[BOARD_SIZE*y + x]]);
            wrefresh(tiles[y][x]);
            rev = ~rev;
        }
        rev = ~rev;
    }
    mvwprintw(stdscr, (TILE_SIZE/2)*BOARD_SIZE + 1, 0, "High-score: %u", high_score);
}

void move_cell(int y, int x,
               int dy, int dx) {
    if (((x + dx >= BOARD_SIZE) || (x + dx < 0)) ||
        ((y + dy >= BOARD_SIZE) || (y + dy < 0))) {
        return;
    }

    if (board[BOARD_SIZE*y + x] == 0) {
        return;
    }

    if (board[BOARD_SIZE*y + x] == board[BOARD_SIZE*(y + dy) + x + dx]) {
        ++board[BOARD_SIZE*(y + dy) + x + dx];
        board[BOARD_SIZE*(y) + x] = 0;
        score += (1 << board[BOARD_SIZE*(y + dy) + x + dx]);
        if (score > high_score) {
            high_score = score;
        }
        change = 1;
    } else if (board[BOARD_SIZE*(y + dy) + x + dx] == 0) {
        board[BOARD_SIZE*(y + dy) + x + dx] = board[BOARD_SIZE*(y) + x];
        board[BOARD_SIZE*(y) + x] = 0;
        change = 1;
        move_cell(y + dy, x + dx, dy, dx);
    }
}

#define move_cell_right(y, x) move_cell(y, x, 0, 1)
#define move_cell_down(y, x) move_cell(y, x, 1, 0)
#define move_cell_left(y, x) move_cell(y, x, 0, -1)
#define move_cell_up(y, x) move_cell(y, x, -1, 0)

void right() {
    int y, x;
    for (y = 0; y < BOARD_SIZE; ++y) {
        for (x = BOARD_SIZE - 2; x >= 0; --x) {
            move_cell_right(y, x);
        }
    }
}

void down() {
    int y, x;
    for (x = 0; x < BOARD_SIZE; ++x) {
        for (y = BOARD_SIZE - 2; y >= 0; --y) {
            move_cell_down(y, x);
        }
    }
}

void left() {
    int y, x;
    for (y = 0; y < BOARD_SIZE; ++y) {
        for (x = 1; x < BOARD_SIZE; ++x) {
            move_cell_left(y, x);
        }
    }
}

void up() {
    int y, x;
    for (x = 0; x < BOARD_SIZE; ++x) {
        for (y = 1; y < BOARD_SIZE; ++y) {
            move_cell_up(y, x);
        }
    }
}

int refresh_free_tiles(void) {
    int i;
    int count = 0;
    for (i = 0; i < BOARD_SIZE*BOARD_SIZE; ++i) {
        if (board[i] == 0) {
            free_tiles[count] = i;
            ++count;
        }
    }
    return count;
}

void insert_random_tile(void) {
    int count = refresh_free_tiles();
    if (count) {
        int r = rand() % count;
        int face = rand() % 2 + 1;
        board[free_tiles[r]] = face;
    }
}

void new_game(void) {
    change = 0;
    score = 0;
    memset(board, 0, (size_t)BOARD_SIZE*BOARD_SIZE);
    insert_random_tile();
    insert_random_tile();
    clear();
    refresh();
}

int main() {
    (void)initscr();

    srand(time(NULL));

    cbreak();
    noecho();
    keypad(stdscr, TRUE);

    init_tiles();

    int last = 0;
    int input = 0;

    if (load_game()) {
        change = 0;
    } else {
        new_game();
    }

 start:
    do {
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
    } while (input != 'q');
    save_game();

    del_tiles();

    endwin();
    return EXIT_SUCCESS;
}
