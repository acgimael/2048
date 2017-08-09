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
extern const char* const save_file_name;

extern struct timespec sleep;

extern const char* relation[];

extern unsigned char board[];

extern int free_tiles[];

extern WINDOW* tiles[BOARD_SIZE][BOARD_SIZE];

void save_game(void);

int load_game(void);

void init_tiles(void);

void del_tiles(void);

void print_board(void);

void side(int y, int x,
          int dy, int dx,
          int ny, int nx);

void backward(int y, int x,
              int dy, int dx);

void forward(int y, int x,
             int dy, int dx);

#define right() side(0, BOARD_SIZE - 2, 0, 1, 1, 0)
#define down() side(BOARD_SIZE - 2, 0, 1, 0, 0, 1)
#define left() side(0, 1, 0, -1, 1, 0)
#define up() side(1, 0, -1, 0, 0, 1)

int refresh_free_tiles(void);

void insert_random_tile(void);

void new_game(void);

void do_initialize_colors(void);

#endif/*HEADER_2048*/
