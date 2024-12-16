//
// Created by alex on 5/31/24.
//

#ifndef CPP3_BRICKGAME_V2_0_1_MASTER_SNAKE_CONTROLLER_H
#define CPP3_BRICKGAME_V2_0_1_MASTER_SNAKE_CONTROLLER_H

#include <iostream>

#include "../brick_game/snake/snake_model.h"
namespace s21 {
class snake_controller {
 public:
  snake_controller() : snake(snake_model()) {}

  void userInput(snake_model::UserAction_t action, bool hold) {
    snake.userInput(action, hold);
  };

  snake_model::GameInfo_t updateCurrentState() {
    return snake.updateCurrentState();
  };

  void setApple(snake_model::Coord_t apple) { snake.setApple(apple); };
  snake_model::GameData_t getGameData() { return snake.game_data; };

 private:
  snake_model snake;
};
}  // namespace s21
#endif  // CPP3_BRICKGAME_V2_0_1_MASTER_SNAKE_CONTROLLER_H
