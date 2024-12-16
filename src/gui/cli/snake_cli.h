//
// Created by alex on 9/5/24.
//

#ifndef CPP3_BRICKGAME_V2_0_1_SNAKE_CLI_H
#define CPP3_BRICKGAME_V2_0_1_SNAKE_CLI_H
#include <ncurses.h>

#include "../../controller/snake_controller.h"
namespace s21 {
class snake_cli {
 public:
  snake_cli() {};
  ~snake_cli() {};
  void print_logo(void);
  void print_game_over(int score);
  void display_field(WINDOW *win, snake_model::GameInfo_t gameInfo);
  // void display_next(WINDOW *win, snake_model::GameData_t *gameData);
  void display_statistics(WINDOW *win, snake_model::GameInfo_t gameInfo);
  void display_help(WINDOW *win);
  int nanosleep(const struct timespec *req, struct timespec *rem);
  void sleep_milli(int milliseconds);
  void change_colors(void);
};
}  // namespace s21
#endif  // CPP3_BRICKGAME_V2_0_1_SNAKE_CLI_H
