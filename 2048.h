#ifndef HEADER_2048
#define HEADER_2048

#include <ncurses.h>

#define BOARD_SIZE 4

extern const int board_size;

extern const char* const title;
extern unsigned int score;
extern unsigned int change;
extern unsigned int high_score;
extern unsigned int enable_colors;
extern unsigned int color;
extern const char* const save_file_name;

extern const char* relation[];

extern unsigned char board[];

extern int free_tiles[];

void save_game(void);

int load_game(void);

extern WINDOW* tiles[BOARD_SIZE][BOARD_SIZE];

#define TILE_SIZE 9

void init_tiles(void);

void del_tiles(void);

void print_board(void);

void move_cell(int y, int x,
	       int dy, int dx);

#define move_cell_right(y, x) move_cell(y, x, 0, 1)
#define move_cell_down(y, x) move_cell(y, x, 1, 0)
#define move_cell_left(y, x) move_cell(y, x, 0, -1)
#define move_cell_up(y, x) move_cell(y, x, -1, 0)

void right(void);

void down(void);

void left(void);

void up(void);

int refresh_free_tiles(void);

void insert_random_tile(void);

void new_game(void);

void do_initialize_colors(void);

#endif/*HEADER_2048*/
