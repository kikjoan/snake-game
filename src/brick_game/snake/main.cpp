//
// Created by alex on 9/5/24.
//
#include <math.h>

#include "../../gui/cli/snake_cli.h"
using namespace s21;

int game_loop(snake_controller *game, snake_cli &gui, WINDOW *field,
              WINDOW *scoreboard, WINDOW *help) {
  int quit = 0;
  bool game_over = true;
  snake_model::GameInfo_t game_info;
  while (game_over) {
    game_info = game->updateCurrentState();
    gui.display_field(field, game_info);
    gui.display_statistics(scoreboard, game_info);
    gui.display_help(help);
    doupdate();
    gui.sleep_milli(10);
    switch (getch()) {
      case KEY_LEFT:
        game->userInput(snake_model::Left, false);
        break;
      case KEY_RIGHT:
        game->userInput(snake_model::Right, false);
        break;
      case ' ':
        game->userInput(snake_model::Start, false);
        break;
      case KEY_DOWN:
        game->userInput(snake_model::Down, false);
        break;
      case KEY_UP:
        game->userInput(snake_model::Up, false);
        break;
      case 'p':
        game->userInput(snake_model::Pause, false);
        break;
      case 'q':
        quit = 1;
        game_over = false;
        break;
        //      case KEY_SLEFT:
        //        game->userInput(snake_model::Action, true);
        //        break;
    }
    wrefresh(field);
    wrefresh(help);
    wrefresh(scoreboard);
    if (game->updateCurrentState().pause == 2) game_over = false;
  }
  return quit;
}

int main() {
  snake_cli gui = snake_cli();
  snake_controller *game = new snake_controller();
  gui.print_logo();
  system("stty cbreak -echo");
  getchar();
  system("stty cooked echo");
  WINDOW *field, *scoreboard, *help;

  initscr();
  cbreak();
  noecho();
  keypad(stdscr, true);
  timeout(0);
  curs_set(0);

  gui.change_colors();

  field = newwin(22, 22, 1, 24);
  help = newwin(16, 16, 8, 2 * (20 + 1) + 22);
  scoreboard = newwin(8, 22, 0, 0);

  while (!game_loop(game, gui, field, scoreboard, help));
  wclear(stdscr);
  delwin(field);
  delwin(help);
  delwin(scoreboard);
  endwin();
  system("clear");

  gui.print_game_over(game->updateCurrentState().score);

  return 0;
}
