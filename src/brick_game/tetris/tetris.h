#ifndef TETRIS_H
#define TETRIS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_LEVEL 10
#define NUMBER_OF_TETROMINOS 7
#define NUMBER_OF_CELLS 4
#define NUMBER_OF_ORIENTATIONS 4
#define HEIGHT 22
#define WIDTH 10
#define POINTS_PER_LEVEL 600

#define max(X, Y) ((X) > (Y) ? (X) : (Y))
#define min(X, Y) ((X) < (Y) ? (X) : (Y))

typedef enum { I, O, T, J, L, S, Z } types_of_tetrominos;

typedef enum { UP, RIGHT, DOWN, LEFT } types_of_orientation;

typedef enum {
  EMPTY_CELL,
  I_CELL,
  O_CELL,
  T_CELL,
  J_CELL,
  L_CELL,
  S_CELL,
  Z_CELL
} types_of_cells;

typedef struct {
  int x;
  int y;
} position;

extern position TETRIS_BRICK[NUMBER_OF_TETROMINOS][NUMBER_OF_ORIENTATIONS]
                            [NUMBER_OF_CELLS];

typedef struct {
  int type;
  int orientation;
  position pos;
} tetromino;

typedef struct {
  int score;
  int level;
  int speed;
  int high_score;
  char **field;
  int points_remain;
  tetromino current_tetromino;
  tetromino next_tetromino;
} GameInfo_t;

typedef enum {
  Start,
  Pause,
  Terminate,
  Left,
  Right,
  Up,
  Down,
  Action
} UserAction_t;

void userInput(UserAction_t action, bool hold);

GameInfo_t updateCurrentState();

extern int LEVEL_TICKRATE[MAX_LEVEL + 1];

char get_cell(GameInfo_t *game, int row, int col);
void set_cell(GameInfo_t *game, int row, int col, char val);
bool is_cell_valid(int row, int col);
void place_tetromino(GameInfo_t *game, tetromino tetromino_to_put);
void remove_tetromino(GameInfo_t *game, tetromino tetromino_to_remove);
int random_tetromino(void);
bool is_tetromino_valid(GameInfo_t *game, tetromino tetromino_to_check);
void generate_new_tetromino(GameInfo_t *game);
void move_tetromino_horizontal(GameInfo_t *game, int direction);
void rotate_tetromino(GameInfo_t *game, int rotation);
void drop_tetromino_down(GameInfo_t *game);
bool is_line_full(GameInfo_t *game, int line_number);
void shift_lines(GameInfo_t *game, int line_number);
int check_for_removals(GameInfo_t *game);
void update_score(GameInfo_t *game, int lines_cleared);
bool is_game_over(GameInfo_t *game);
void make_movement(GameInfo_t *game, UserAction_t movement);
void make_a_step(GameInfo_t *game);
bool tick(GameInfo_t *game, UserAction_t movement);
void init_game(GameInfo_t *game, int field_height, int field_width);
GameInfo_t *create_game(int height, int width);
void clean_memory(GameInfo_t *game);
#ifdef __cplusplus
}
#endif
#endif  // TETRIS_H