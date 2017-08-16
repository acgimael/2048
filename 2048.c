#include "2048.h"

#include <stdlib.h>
#include <string.h>

#define ARR_LEN(arr) (sizeof((arr))/sizeof((arr)[0]))
#define BOARD(y, x) board[BOARD_SIZE*(y) + (x)]

const int board_size = BOARD_SIZE;

const char* const title = "2048";

unsigned int score = 0;
unsigned int change = 0;
unsigned int high_score = 0;

unsigned int enable_colors = 1;
unsigned int color = 0;

unsigned int game_over = 0;

struct timespec sleep = {0, 150000000};
struct timespec anim8 = {0,  37500000};

const char* const save_file_name = "save.game";

const char* relation[] = {
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

unsigned char board[BOARD_SIZE*BOARD_SIZE] = {0};
int free_tiles[BOARD_SIZE*BOARD_SIZE];

WINDOW* tiles[BOARD_SIZE][BOARD_SIZE];

void save_game(void) {
    FILE* fp = fopen(save_file_name, "wb");
    if (fp) {
        if (fwrite(&board_size, sizeof(board_size), 1, fp) != 1) {
            fputs("Could not write the board size "
                  "to the save game file\n", stderr);
            exit(EXIT_FAILURE);
        }
        if (fwrite(&score, sizeof(score), 1, fp) != 1) {
            fputs("Could not write the score "
                  "to the save game file\n", stderr);
            exit(EXIT_FAILURE);
        }
        if (fwrite(&high_score, sizeof(high_score), 1, fp) != 1) {
            fputs("Could not write the high score "
                  "to the save game file\n", stderr);
            exit(EXIT_FAILURE);
        }
        if (fwrite(board, sizeof(*board), board_size*board_size, fp) !=
            BOARD_SIZE*BOARD_SIZE) {
            fputs("Could not write the board data "
                  "to the save game file\n", stderr);
            exit(EXIT_FAILURE);
        }
        fclose(fp);
    }
}

int load_game(void) {
    int loaded = 0;
    FILE* fp = fopen(save_file_name, "rb");
    if (fp) {
        int read_board_size = 0;
        if (fread(&read_board_size, sizeof(board_size), 1, fp) != 1) {
            fputs("Could not read the board size "
                  "from the save game file\n", stderr);
            exit(EXIT_FAILURE);
        } else {
            if (read_board_size != board_size) {
                fputs("Save game board size and "
                      "actual board size do not match\n", stderr);
                exit(EXIT_FAILURE);
            }
        }
        if (fread(&score, sizeof(score), 1, fp) != 1) {
            fputs("Could not read the score "
                  "from the save game file\n", stderr);
            exit(EXIT_FAILURE);
        }
        if (fread(&high_score, sizeof(high_score), 1, fp) != 1) {
            fputs("Could not read the high score "
                  "from the save game file\n", stderr);
            exit(EXIT_FAILURE);
        }
        if (fread(board, sizeof(*board), board_size*board_size, fp) !=
            BOARD_SIZE*BOARD_SIZE) {
            fputs("Could not read the board data "
                  "from the save game file\n", stderr);
            exit(EXIT_FAILURE);
        }
        loaded = 1;
        fclose(fp);
    }
    return loaded;
}

void is_game_over() {
    int y, x;
    for (y = 0; y < BOARD_SIZE; ++y) {
        for (x = 0; x < BOARD_SIZE; ++x) {
            /*right*/ if ((x + 1 < BOARD_SIZE) && BOARD(y, x) == BOARD(y, x + 1)) return;
            /*down */ if ((y + 1 < BOARD_SIZE) && BOARD(y, x) == BOARD(y + 1, x)) return;
        }
    }
    game_over = 1;
}

void init_tiles(void) {
    int y, x;
    for (y = 0; y < board_size; ++y) {
        for (x = 0; x < board_size; ++x) {
            tiles[y][x] = subwin(stdscr,
                                 TILE_SIZE/2, TILE_SIZE,
                                 1 + y*(TILE_SIZE/2), x*TILE_SIZE);
        }
    }
}

void del_tiles(void) {
    int y, x;
    for (y = 0; y < board_size; ++y) {
        for (x = 0; x < board_size; ++x) {
            delwin(tiles[y][x]);
        }
    }
}

void print_board(void) {
    int y, x;
    int rev = 0;
    (void)mvwaddstr(stdscr, 0, 1, title);
    (void)mvwprintw(stdscr, 0, 2*TILE_SIZE, "score: %d", score);
    for (y = 0; y < board_size; ++y) {
        for (x = 0; x < board_size; ++x) {
            if (color) {
                if (BOARD(y, x) < ARR_LEN(relation)) {
                    wbkgd(tiles[y][x], ' ' |
                          COLOR_PAIR(BOARD(y, x)));
                } else {
                    wbkgd(tiles[y][x], ' ' | COLOR_PAIR(0));
                }
            } else {
                wbkgd(tiles[y][x], ' ' | ((rev)?A_REVERSE:0));
            }
            if (BOARD(y, x) < ARR_LEN(relation)) {
                mvwprintw(tiles[y][x],
                          TILE_SIZE/4, 1,
                          "%7s",
                          relation[(size_t)BOARD(y, x)]);
            } else {
                mvwprintw(tiles[y][x],
                          TILE_SIZE/4, 1,
                          "2^%d",
                          BOARD(y, x));
            }
            wrefresh(tiles[y][x]);
            rev = ~rev;
        }
        rev = ~rev;
    }
    mvwprintw(stdscr,
              (TILE_SIZE/2)*BOARD_SIZE + 1, 0,
              "High-score: %u%s",
              high_score, (game_over)?" [GAME OVER]":"");
}

void move_tile(int y, int x,
               int dy, int dx) {
    if (BOARD(y, x) == 0) {
        return;
    }

    if (((x + dx >= BOARD_SIZE) || (x + dx < 0))
        || ((y + dy >= BOARD_SIZE) || (y + dy < 0))) {
        return;
    }

    if (BOARD(y + dy, x + dx) == 0) {
        change = 1;
        BOARD(y + dy, x + dx) = BOARD(y, x);
        BOARD(y, x) = 0;

        print_board();
        refresh();

        nanosleep(&anim8, NULL);

        move_tile(y + dy, x + dx, dy, dx);
    }
}

void right(void) {
    int y, x;
    for (x = BOARD_SIZE - 2; x >= 0; --x) {
        for (y = 0; y < BOARD_SIZE; ++y) {
            move_tile(y, x, 0, 1);
        }
    }
}

void merge_right(void) {
    int y, x;
    for (x = BOARD_SIZE - 2; x >= 0; --x) {
        for (y = 0; y < BOARD_SIZE; ++y) {
            merge_tiles(y, x, 0, 1);
        }
    }
}

void down(void) {
    int y, x;
    for (y = BOARD_SIZE - 2; y >= 0; --y) {
        for (x = 0; x < BOARD_SIZE; ++x) {
            move_tile(y, x, 1, 0);
        }
    }
}

void merge_down(void) {
    int y, x;
    for (y = BOARD_SIZE - 2; y >= 0; --y) {
        for (x = 0; x < BOARD_SIZE; ++x) {
            merge_tiles(y, x, 1, 0);
        }
    }
}

void left(void) {
    int y, x;
    for (x = 1; x < BOARD_SIZE; ++x) {
        for (y = 0; y < BOARD_SIZE; ++y) {
            move_tile(y, x, 0, -1);
        }
    }
}

void merge_left(void) {
    int y, x;
    for (x = 1; x < BOARD_SIZE; ++x) {
        for (y = 0; y < BOARD_SIZE; ++y) {
            merge_tiles(y, x, 0, -1);
        }
    }
}

void up(void) {
    int y, x;
    for (y = 1; y < BOARD_SIZE; ++y) {
        for (x = 0; x < BOARD_SIZE; ++x) {
            move_tile(y, x, -1, 0);
        }
    }
}

void merge_up(void) {
    int y, x;
    for (y = 1; y < BOARD_SIZE; ++y) {
        for (x = 0; x < BOARD_SIZE; ++x) {
            merge_tiles(y, x, -1, 0);
        }
    }
}

void merge_tiles(int y, int x,
                 int dy, int dx) {
    if (BOARD(y, x) == 0) {
        return;
    }

    if (((x + dx >= BOARD_SIZE) || (x + dx < 0))
        || ((y + dy >= BOARD_SIZE) || (y + dy < 0))) {
        return;
    }

    if (BOARD(y + dy, x + dx) == BOARD(y, x)) {
        change = 1;
        ++BOARD(y + dy, x + dx);
        score += (1 << BOARD(y + dy, x + dx));
        BOARD(y, x) = 0;
        if (score > high_score) {
            high_score = score;
        }


        print_board();
        refresh();

        nanosleep(&anim8, NULL);
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
        int face = ((rand() % 10) < 9)?1:2;
        board[free_tiles[r]] = face;
    }
}

void new_game(void) {
    game_over = 0;
    change = 0;
    score = 0;
    memset(board, 0, (size_t)board_size*board_size);
    insert_random_tile();
    insert_random_tile();
    clear();
    refresh();
}

void do_initialize_colors(void) {
    start_color();
    color = 1;
    init_pair(1, COLOR_BLACK, COLOR_RED);
    init_pair(2, COLOR_BLACK, COLOR_GREEN);
    init_pair(3, COLOR_BLACK, COLOR_YELLOW);
    init_pair(4, COLOR_BLACK, COLOR_BLUE);
    init_pair(5, COLOR_BLACK, COLOR_MAGENTA);
    init_pair(6, COLOR_BLACK, COLOR_CYAN);
    init_pair(7, COLOR_BLACK, COLOR_WHITE);
    init_pair(8, COLOR_RED, COLOR_BLACK);
    init_pair(9, COLOR_RED, COLOR_GREEN);
    init_pair(10, COLOR_RED, COLOR_YELLOW);
    init_pair(11, COLOR_RED, COLOR_BLUE);
    init_pair(12, COLOR_RED, COLOR_MAGENTA);
    init_pair(13, COLOR_RED, COLOR_CYAN);
    init_pair(14, COLOR_RED, COLOR_WHITE);
    init_pair(15, COLOR_GREEN, COLOR_RED);
    init_pair(16, COLOR_GREEN, COLOR_WHITE);
    init_pair(17, COLOR_GREEN, COLOR_YELLOW);
    init_pair(18, COLOR_GREEN, COLOR_BLUE);
    init_pair(19, COLOR_GREEN, COLOR_MAGENTA);
    init_pair(20, COLOR_GREEN, COLOR_CYAN);
    init_pair(21, COLOR_GREEN, COLOR_WHITE);
    init_pair(22, COLOR_YELLOW, COLOR_RED);
    init_pair(23, COLOR_YELLOW, COLOR_GREEN);
    init_pair(24, COLOR_YELLOW, COLOR_BLACK);
    init_pair(25, COLOR_YELLOW, COLOR_BLUE);
    init_pair(26, COLOR_YELLOW, COLOR_MAGENTA);
    init_pair(27, COLOR_YELLOW, COLOR_CYAN);
    init_pair(28, COLOR_YELLOW, COLOR_WHITE);
}
