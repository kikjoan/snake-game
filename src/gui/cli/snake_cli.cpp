//
// Created by alex on 9/5/24.
//

#include "snake_cli.h"

#include <time.h>
using namespace s21;
void snake_cli::sleep_milli(int milliseconds) {
  struct timespec ts;
  ts.tv_sec = 0;
  ts.tv_nsec = milliseconds * 1000 * 1000;
  //clock_nanosleep(1, CLOCK_MONOTONIC, &ts, NULL);
}

void snake_cli::display_field(WINDOW *win, snake_model::GameInfo_t game) {
  box(win, 0, 0);
  wmove(win, 0, getmaxx(win) / 2 - 2);
  wprintw(win, "SNAKE");
  for (int i = 0; i < 20; ++i) {
    wmove(win, i + 1, 1);
    for (int j = 0; j < 10; ++j) {
      if (game.field[i][j] != snake_model::Field) {
        waddch(win, ' ' | A_REVERSE | COLOR_PAIR(game.field[i][j]));
        waddch(win, ' ' | A_REVERSE | COLOR_PAIR(game.field[i][j]));
      } else {
        waddch(win, ' ');
        waddch(win, ' ');
      }
    }
  }
  wnoutrefresh(win);
}

void snake_cli::display_statistics(WINDOW *win, snake_model::GameInfo_t game) {
  box(win, 0, 0);
  wmove(win, 0, getmaxx(win) / 2 - 2);
  wprintw(win, "STATS");
  wmove(win, 1, 2);
  wprintw(win, "SCORE:\t%d", game.score);
  wmove(win, 3, 2);
  wprintw(win, "HIGHSCORE:\t%d", game.high_score);
  wmove(win, 5, 2);
  wprintw(win, "LEVEL:\t%d", game.level);
  wnoutrefresh(win);
}

void snake_cli::display_help(WINDOW *win) {
  box(win, 0, 0);
  wmove(win, 0, getmaxx(win) / 2 - 1);
  wprintw(win, "HELP");
  wmove(win, 1, 2);
  wprintw(win, "Left\t    <");
  wmove(win, 2, 2);
  wprintw(win, "Right\t    >");
  wmove(win, 3, 2);
  wprintw(win, "Up\t    ^");
  wmove(win, 4, 2);
  wprintw(win, "Down\t    v");
  wmove(win, 6, 2);
  wprintw(win, "Start\t space");
  wmove(win, 8, 2);
  wprintw(win, "Pause\t p");
  wmove(win, 10, 2);
  wprintw(win, "Quit\t q");
  // wmove(win, 12, 2);
  //   wprintw(win, "Boost\t s");
  wnoutrefresh(win);
}

void snake_cli::print_game_over(int score) {
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

void snake_cli::print_logo(void) {
  printf("\t\t███████╗███╗   ██╗ █████╗ ██╗  ██╗███████╗\n");
  printf("\t\t██╔════╝████╗  ██║██╔══██╗██║ ██╔╝██╔════╝\n");
  printf("\t\t███████╗██╔██╗ ██║███████║█████╔╝ █████╗  \n");
  printf("\t\t╚════██║██║╚██╗██║██╔══██║██╔═██╗ ██╔══╝  \n");
  printf("\t\t███████║██║ ╚████║██║  ██║██║  ██╗███████╗\n");
  printf("\t\t╚══════╝╚═╝  ╚═══╝╚═╝  ╚═╝╚═╝  ╚═╝╚══════╝\n\n");
  printf("\t\t\tUse arrows to move, q to quit\n");
  printf("\t\t\tPress any key to continue...\n");
}

void snake_cli::change_colors(void) {
  start_color();
  init_pair(0, COLOR_BLUE, COLOR_BLACK);
  init_pair(1, COLOR_GREEN, COLOR_BLACK);
  init_pair(2, COLOR_RED, COLOR_BLACK);
}
