#ifndef CLI_H
#define CLI_H

#include <ncurses.h>

#include "../../brick_game/tetris/tetris.h"

#define COLOR_ORANGE 8

void print_logo(void);
void print_game_over(int score);
void display_field(WINDOW *win, GameInfo_t *game);
void display_next(WINDOW *win, tetromino tetromino_to_display);
void display_statistics(WINDOW *win, GameInfo_t *game);
void display_help(WINDOW *win);
int nanosleep(const struct timespec *req, struct timespec *rem);
void sleep_milli(int milliseconds);
void change_colors(void);

#endif  // CLI_H