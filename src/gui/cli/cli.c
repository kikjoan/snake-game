#include "cli.h"

void sleep_milli(int milliseconds) {
  struct timespec ts;
  ts.tv_sec = 0;
  ts.tv_nsec = milliseconds * 1000 * 1000;
  nanosleep(&ts, NULL);
}

void display_field(WINDOW *win, GameInfo_t *game) {
  box(win, 0, 0);
  wmove(win, 0, getmaxx(win) / 2 - 2);
  wprintw(win, "TETRIS");
  for (int i = 0; i < HEIGHT; ++i) {
    wmove(win, i + 1, 1);
    for (int j = 0; j < WIDTH; ++j) {
      if (get_cell(game, i, j) != EMPTY_CELL) {
        waddch(win, ' ' | A_REVERSE | COLOR_PAIR(get_cell(game, i, j)));
        waddch(win, ' ' | A_REVERSE | COLOR_PAIR(get_cell(game, i, j)));
      } else {
        waddch(win, ' ');
        waddch(win, ' ');
      }
    }
  }
  wnoutrefresh(win);
}

void display_next(WINDOW *win, tetromino tetromino_to_display) {
  wclear(win);
  box(win, 0, 0);
  wmove(win, 0, getmaxx(win) / 2 - 1);
  wprintw(win, "NEXT");
  position pos;
  for (int i = 0; i < NUMBER_OF_CELLS; ++i) {
    pos = TETRIS_BRICK[tetromino_to_display.type]
                      [tetromino_to_display.orientation][i];
    wmove(win, pos.y + 2, pos.x * 2 + 3);
    waddch(win, ' ' | A_REVERSE | COLOR_PAIR(tetromino_to_display.type + 1));
    waddch(win, ' ' | A_REVERSE | COLOR_PAIR(tetromino_to_display.type + 1));
  }
  wnoutrefresh(win);
}

void display_statistics(WINDOW *win, GameInfo_t *game) {
  box(win, 0, 0);
  wmove(win, 0, getmaxx(win) / 2 - 2);
  wprintw(win, "STATS");
  wmove(win, 1, 2);
  wprintw(win, "SCORE:\t%d", game->score);
  wmove(win, 3, 2);
  wprintw(win, "HIGHSCORE:\t%d", game->high_score);
  wmove(win, 5, 2);
  wprintw(win, "LEVEL:\t%d", game->level);
  wnoutrefresh(win);
}

void display_help(WINDOW *win) {
  box(win, 0, 0);
  wmove(win, 0, getmaxx(win) / 2 - 1);
  wprintw(win, "HELP");
  wmove(win, 1, 2);
  wprintw(win, "Left\t    <-");
  wmove(win, 3, 2);
  wprintw(win, "Right\t    ->");
  wmove(win, 5, 2);
  wprintw(win, "Drop\t    ");
  waddch(win, ACS_DARROW);
  wmove(win, 7, 2);
  wprintw(win, "Rotate  space");
  wmove(win, 9, 2);
  wprintw(win, "Pause\t    p");
  wmove(win, 11, 2);
  wprintw(win, "Quit\t    q");
  wnoutrefresh(win);
}

void print_game_over(int score) {
  printf(
      " ██████╗  █████╗ ███╗   ███╗███████╗     ██████╗ ██╗   "
      "██╗███████╗██████╗ \n");
  printf(
      "██╔════╝ ██╔══██╗████╗ ████║██╔════╝    ██╔═══██╗██║   "
      "██║██╔════╝██╔══██╗ \n");
  printf(
      "██║  ███╗███████║██╔████╔██║█████╗      ██║   ██║██║   ██║█████╗  "
      "██████╔╝ \n");
  printf(
      "██║   ██║██╔══██║██║╚██╔╝██║██╔══╝      ██║   ██║╚██╗ ██╔╝██╔══╝  "
      "██╔══██╗ \n");
  printf(
      "╚██████╔╝██║  ██║██║ ╚═╝ ██║███████╗    ╚██████╔╝ ╚████╔╝ ███████╗██║  "
      "██║ \n");
  printf(
      " ╚═════╝ ╚═╝  ╚═╝╚═╝     ╚═╝╚══════╝     ╚═════╝   ╚═══╝  ╚══════╝╚═╝  "
      "╚═╝ \n\n");
  printf("\t\t\tYou scored %d points!\n", score);
}

void print_logo(void) {
  printf("\t\t████████╗███████╗████████╗██████╗ ██╗███████╗\n");
  printf("\t\t╚══██╔══╝██╔════╝╚══██╔══╝██╔══██╗██║██╔════╝\n");
  printf("\t\t   ██║   █████╗     ██║   ██████╔╝██║███████╗\n");
  printf("\t\t   ██║   ██╔══╝     ██║   ██╔══██╗██║╚════██║\n");
  printf("\t\t   ██║   ███████╗   ██║   ██║  ██║██║███████║\n");
  printf("\t\t   ╚═╝   ╚══════╝   ╚═╝   ╚═╝  ╚═╝╚═╝╚══════╝\n\n");
  printf("\t\t\tUse arrows to move, q to quit\n");
  printf("\t\t\tPress any key to continue...\n");
}

void change_colors(void) {
  start_color();
  init_color(COLOR_ORANGE, 1000, 400, 0);
  init_color(COLOR_YELLOW, 1000, 1000, 0);
  init_color(COLOR_MAGENTA, 1000, 0, 1000);
  init_color(COLOR_GREEN, 0, 500, 0);
  init_color(COLOR_BLUE, 0, 302, 1000);
  init_color(COLOR_RED, 1000, 0, 0);
  init_color(COLOR_CYAN, 0, 1000, 1000);
  init_pair(S_CELL, COLOR_GREEN, COLOR_BLACK);
  init_pair(O_CELL, COLOR_YELLOW, COLOR_BLACK);
  init_pair(Z_CELL, COLOR_RED, COLOR_BLACK);
  init_pair(J_CELL, COLOR_BLUE, COLOR_BLACK);
  init_pair(I_CELL, COLOR_CYAN, COLOR_BLACK);
  init_pair(T_CELL, COLOR_MAGENTA, COLOR_BLACK);
  init_pair(L_CELL, COLOR_ORANGE, COLOR_BLACK);
}