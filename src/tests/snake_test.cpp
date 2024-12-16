//
// Created by alex on 8/30/24.
//

#include <gtest/gtest.h>

#include <thread>

#include "../brick_game/snake/snake_model.h"
#include "../brick_game/tetris/tetris.h"
#include "../controller/snake_controller.h"
using namespace s21;

TEST(SnakeModelTest, SnakeModelTest1) {
  snake_controller controller;
  controller.userInput(snake_model::Start, false);
  std::this_thread::sleep_for(std::chrono::seconds(2));
  snake_model::GameInfo_t info = controller.updateCurrentState();
  controller.userInput(snake_model::Up, false);
  std::this_thread::sleep_for(std::chrono::seconds(1));
  controller.userInput(snake_model::Left, false);
  std::this_thread::sleep_for(std::chrono::seconds(1));
  controller.userInput(snake_model::Down, false);
  std::this_thread::sleep_for(std::chrono::seconds(1));
  controller.userInput(snake_model::Right, false);
  info = controller.updateCurrentState();
  EXPECT_EQ(info.pause, 2);
  controller.userInput(snake_model::Terminate, false);
  std::this_thread::sleep_for(std::chrono::seconds(1));
}

TEST(SnakeModelTest, SnakeModelTest2) {
  snake_controller controller;
  snake_model::GameInfo_t info = controller.updateCurrentState();
  controller.userInput(snake_model::Start, false);
  std::this_thread::sleep_for(std::chrono::seconds(1));
  EXPECT_EQ(info.pause, 1);
  controller.userInput(snake_model::Pause, false);
  info = controller.updateCurrentState();
  std::this_thread::sleep_for(std::chrono::seconds(3));
  EXPECT_EQ(info.pause, 2);
  controller.userInput(snake_model::Terminate, false);
  std::this_thread::sleep_for(std::chrono::seconds(1));
  info = controller.updateCurrentState();
  std::this_thread::sleep_for(std::chrono::seconds(1));
  EXPECT_EQ(info.pause, 1);
}

TEST(SnakeModelTest, SnakeModelTest3) {
  snake_controller controller;
  controller.userInput(snake_model::Start, false);
  std::this_thread::sleep_for(std::chrono::seconds(1));
  snake_model::GameInfo_t info = controller.updateCurrentState();
  info = controller.updateCurrentState();
  EXPECT_EQ(info.pause, 2);
  controller.userInput(snake_model::Up, false);
  controller.userInput(snake_model::Action, true);
  controller.userInput(snake_model::Action, false);
  controller.userInput(snake_model::Left, false);
  std::this_thread::sleep_for(std::chrono::seconds(15));
  info = controller.updateCurrentState();
  EXPECT_EQ(info.pause, 2);
}

TEST(SnakeModelTest, SnakeModelTest4) {
  snake_controller controller;
  controller.userInput(snake_model::Start, false);
  snake_controller controller2(controller);
  std::this_thread::sleep_for(std::chrono::seconds(1));
  controller2.userInput(snake_model::Start, false);
  std::this_thread::sleep_for(std::chrono::seconds(10));
  snake_model::GameInfo_t info = controller.updateCurrentState();
  snake_model::GameInfo_t info2 = controller2.updateCurrentState();
  EXPECT_EQ(info.pause, info2.pause);
  EXPECT_EQ(info.score, info2.score);
  for (int y = 0; y < 20; y++) {
    for (int x = 0; x < 10; x++) {
      EXPECT_EQ(info.field[y][x], info2.field[y][x]);
    }
  }
}

TEST(SnakeModelTest, SnakeModelTest5) {
  snake_controller controller;
  controller.userInput(snake_model::Start, false);
  snake_controller controller2 = controller;
  std::this_thread::sleep_for(std::chrono::seconds(1));
  controller2.userInput(snake_model::Start, false);
  std::this_thread::sleep_for(std::chrono::seconds(10));
  snake_model::GameInfo_t info = controller.updateCurrentState();
  snake_model::GameInfo_t info2 = controller2.updateCurrentState();
  EXPECT_EQ(info.pause, info2.pause);
  EXPECT_EQ(info.score, info2.score);
  for (int y = 0; y < 20; y++) {
    for (int x = 0; x < 10; x++) {
      EXPECT_EQ(info.field[y][x], info2.field[y][x]);
    }
  }
}

TEST(SnakeModelTest, SnakeModelTest6) {
  snake_controller controller;
  controller.userInput(snake_model::Start, false);
  controller.userInput(snake_model::Pause, false);
  snake_model::GameData_t data = controller.getGameData();
  snake_model::GameInfo_t info = controller.updateCurrentState();
  while (true) {
    switch (data.head_dir) {
      case snake_model::UserAction_t::Up:
        if (data.snake[0].y - 1 >= 0)
          controller.setApple(
              snake_model::Coord_t{data.snake[0].x, data.snake[0].y - 1});
        break;

      case snake_model::UserAction_t::Down:
        if (data.snake[0].y + 1 <= 20)
          controller.setApple(
              snake_model::Coord_t{data.snake[0].x, data.snake[0].y + 1});
        break;

      case snake_model::UserAction_t::Left:
        if (data.snake[0].x - 1 >= 0)
          controller.setApple(
              snake_model::Coord_t{data.snake[0].x - 1, data.snake[0].y});
        break;

      case snake_model::UserAction_t::Right:
        if (data.snake[0].x + 1 <= 10)
          controller.setApple(
              snake_model::Coord_t{data.snake[0].x + 1, data.snake[0].y});
        break;
    }
    controller.userInput(snake_model::Start, false);
    std::this_thread::sleep_for(std::chrono::milliseconds(120));
    switch (data.head_dir) {
      case Right:
        controller.userInput(snake_model::Up, false);
        break;
      case Down:
        controller.userInput(snake_model::Right, false);
        break;
      case Left:
        controller.userInput(snake_model::Down, false);
        break;
      case Up:
        controller.userInput(snake_model::Left, false);
        break;
      default:
        break;
    }
    // std::this_thread::sleep_for(std::chrono::milliseconds(210));
    info = controller.updateCurrentState();
    data = controller.getGameData();
    if (info.pause == 2) {
      break;
    } else
      controller.userInput(snake_model::Pause, false);
  }
  controller.userInput(snake_model::Start, false);
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}