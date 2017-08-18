#ifndef HEADER_2048
#define HEADER_2048

#include <ncurses.h>
#include <time.h>

#define BOARD_SIZE 4

#define TILE_SIZE 9

extern const int board_size;

extern const char* const title;
extern unsigned int score;
extern unsigned int change;
extern unsigned int high_score;
extern unsigned int enable_colors;
extern unsigned int color;
extern unsigned int game_over;
extern const char* const save_file_name;

extern struct timespec sleep;

extern const char* relation[];

extern unsigned char board[];

extern int free_tiles[];

extern WINDOW* tiles[BOARD_SIZE][BOARD_SIZE];

typedef enum direction {
    right, down, left, up
} direction;

void save_game(void);

int load_game(void);

void is_game_over(void);

void init_tiles(void);

void del_tiles(void);

void print_board(void);

void move_tile(int y, int x,
               int dy, int dx);

void board_move(direction dir);

void merge_tiles(int y, int x,
                 int dy, int dx);

int refresh_free_tiles(void);

void insert_random_tile(void);

void new_game(void);

void do_initialize_colors(void);

#endif/*HEADER_2048*/
