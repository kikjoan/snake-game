//
// Created by alex on 5/25/24.
//

#ifndef CPP3_BRICKGAME_V2_0_1_MASTER_SNAKE_MODEL_H
#define CPP3_BRICKGAME_V2_0_1_MASTER_SNAKE_MODEL_H

#include <vector>
namespace s21 {
class snake_controller;
/**
 * @class snake_model is the model of the snake game. It contains the game logic
 * and the game state.
 */
class snake_model {
  friend snake_controller;

 public:
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

  typedef struct {
    int **field;
    int **next;
    int score;
    int high_score;
    int level;
    int speed;
    int pause;
  } GameInfo_t;

  typedef enum { Head, Body, Apple, Field } ObjectType_t;

  typedef struct {
    int x;
    int y;
  } Coord_t;

  /**
   * head_dir(up: 5, right: 4, down: 6, left: 3)
   */
  typedef struct {
    Coord_t apple;
    int head_dir;
    bool isRotate;
    bool isSonic;
    std::vector<Coord_t> snake;
  } GameData_t;

  snake_model();
  ~snake_model();
  // snake_model &operator=(snake_model const &s);
  GameInfo_t updateCurrentState();
  snake_model(const snake_model &model);

 protected:
  GameInfo_t game_info;
  GameData_t game_data;
  int speed[10]{200, 195, 190, 185, 180, 175, 170, 165, 160, 155};
  void preStart();
  void start();
  void terminate();
  void gameOver();
  void readSave();
  void writeSave();
  void setDefaultField();
  void move();
  bool isCollision();
  bool isApple();
  void spawnApple();
  void fillField();
  void userInput(snake_model::UserAction_t action, bool hold);
  int getSpeed();
  void setApple(Coord_t apple);
};
}  // namespace s21
#endif  // CPP3_BRICKGAME_V2_0_1_MASTER_SNAKE_MODEL_H
