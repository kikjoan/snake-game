//
// Created by alex on 5/25/24.
//

#include "snake_model.h"

// #include <QThread>
#include <cstring>
#include <fstream>
#include <iostream>
#include <random>
#include <thread>
#include <vector>

using namespace s21;
/**
 * @brief Constructor for the snake_model class.
 * Initializes the game field and sets the game to the pre-start state.
 */
snake_model::snake_model() {
  game_info.field = new int*[20];
  for (int y = 0; y < 20; y++) {
    game_info.field[y] = new int[10];
    for (int x = 0; x < 10; x++) {
      game_info.field[y][x] = Field;
    }
  }
  game_info.pause = 1;
  preStart();
}
/**
 * @brief Destructor for the snake_model class.
 * Cleans up dynamically allocated memory for the game field.
 */
snake_model::~snake_model() {
  for (int i = 0; i < 20; i++) {
    delete[] game_info.field[i];
  }
  delete[] game_info.field;
  /// delete[] speed;
}

/**
 * @brief Updates the current state of the game.
 * @return The current game information.
 */
snake_model::GameInfo_t snake_model::updateCurrentState() {
  //  while (!s21::snake_model::action_mutex.try_lock()) {
  //    QThread::msleep(5);
  //  }
  fillField();
  // s21::snake_model::action_mutex.unlock();
  return game_info;
}

/**
 * @brief Resets the game variables to their initial state.
 */
void snake_model::preStart() {
  game_info.score = 0;
  game_info.next = nullptr;
  game_info.level = 1;
  game_info.speed = 150;
  game_info.pause = 1;
  game_data.isRotate = false;
  game_data.apple.x = 0;
  game_data.apple.y = 0;
  game_data.head_dir = Right;
  game_data.isRotate = false;
  game_data.isSonic = false;
  setDefaultField();
  readSave();
}

/**
 * @brief Starts the game loop.
 * The game loop runs until the game is paused or a collision occurs.
 */
void snake_model::start() {
  if (game_info.pause) {
    game_info.pause = 0;
    while (!game_info.pause) {
      // sleep threed
      std::this_thread::sleep_for(std::chrono::milliseconds(game_info.speed));
      // QThread::msleep(game_info.speed);
      //       while (!s21::snake_model::action_mutex.try_lock()) {
      //         QThread::msleep(2);
      //       }
      if (isCollision()) {
        gameOver();
      } else {
        move();
        game_data.isRotate = false;
        game_info.speed = getSpeed();
      }
      //      action_mutex.unlock();
    }
  }
}

/**
 * @brief Terminates the game and resets it to the pre-start state.
 */
void snake_model::terminate() { preStart(); }

/**
 * @brief Handles the game over state.
 * Pauses the game and saves the high score if applicable.
 */
void snake_model::gameOver() {
  game_info.pause = 2;
  if (game_info.score > game_info.high_score) {
    writeSave();
  }
  // preStart();
}

/**
 * @brief Checks if the snake has collided with the wall or its own body.
 * @return True if a collision occurred, false otherwise.
 */
bool snake_model::isCollision() {
  switch (game_data.head_dir) {
    case Left:
      if (game_data.snake[0].x == 0 ||
          (game_info.field[game_data.snake[0].y][game_data.snake[0].x - 1] ==
           Body))
        return true;
      break;
    case Right:
      if (game_data.snake[0].x == 9 ||
          (game_info.field[game_data.snake[0].y][game_data.snake[0].x + 1] ==
           Body))
        return true;
      break;
    case Up:
      if (game_data.snake[0].y == 0 ||
          (game_info.field[game_data.snake[0].y - 1][game_data.snake[0].x] ==
           Body))
        return true;
      break;
    case Down:
      if (game_data.snake[0].y == 19 ||
          (game_info.field[game_data.snake[0].y + 1][game_data.snake[0].x] ==
           Body))
        return true;
      break;
    default:
      return false;
  }
  return false;
}

/**
 * @brief Checks if the snake has eaten an apple.
 * @return True if the snake has eaten an apple, false otherwise.
 */
bool snake_model::isApple() {
  bool apple = false;
  switch (game_data.head_dir) {
    case Left:
      if (game_data.snake[0].x - 1 == game_data.apple.x &&
          game_data.snake[0].y == game_data.apple.y)
        apple = true;
      break;
    case Right:
      if (game_data.snake[0].x + 1 == game_data.apple.x &&
          game_data.snake[0].y == game_data.apple.y)
        apple = true;
      break;
    case Up:
      if (game_data.snake[0].y - 1 == game_data.apple.y &&
          game_data.snake[0].x == game_data.apple.x)
        apple = true;
      break;
    case Down:
      if (game_data.snake[0].y + 1 == game_data.apple.y &&
          game_data.snake[0].x == game_data.apple.x)
        apple = true;
      break;
    default:
      apple = false;
  }
  if (apple) {
    game_info.score++;
    spawnApple();
  }
  return apple;
}

// начальное положение змейки в векторе
void snake_model::setDefaultField() {
  game_data.snake.clear();
  game_data.snake.push_back({5, 17});
  game_data.snake.push_back({4, 17});
  game_data.snake.push_back({3, 17});
  game_data.apple = {3, 4};
  game_data.head_dir = Right;
}

/**
 * @brief Sets the default position of the snake and the apple on the field.
 */
void snake_model::move() {
  Coord_t tail = game_data.snake.back();
  bool apple = isApple();
  for (unsigned long i = game_data.snake.size() - 1; i > 0; i--)
    game_data.snake[i] = game_data.snake[i - 1];

  if (apple) game_data.snake.push_back(tail);
  switch (game_data.head_dir) {
    case Left:
      game_data.snake[0].x--;
      break;
    case Right:
      game_data.snake[0].x++;
      break;
    case Up:
      game_data.snake[0].y--;
      break;
    case Down:
      game_data.snake[0].y++;
      break;
  }
  game_data.isRotate = false;
}

/**
 * @brief Moves the snake in the current direction.
 * If the snake eats an apple, it grows in length.
 */
void snake_model::readSave() {
  std::ifstream file;
  int score;
  file.open("save.dat", std::ios::in);
  if (!file.is_open()) {
    std::cout << "Файл не найден" << std::endl;
    std::ofstream of = std::ofstream("save.dat");
    of << 0;
    of.close();
    file.open("save.dat", std::ios::in);
  }
  file >> score;
  file.close();
  game_info.high_score = score;
}

/**
 * @brief Writes the current score to a save file.
 */
void snake_model::writeSave() {
  std::ofstream file;
  file.open("save.dat", std::ios::out);
  file << game_info.score;
  std::cout << "Записал" << std::endl;
  file.close();
}

/**
 * @brief Spawns a new apple at a random free spot on the field.
 */
void snake_model::spawnApple() {
  std::vector<Coord_t> freeSpots;
  freeSpots.reserve(200);
  for (int y = 0; y < 20; y++) {
    for (int x = 0; x < 10; x++) {
      if (game_info.field[y][x] == Field) {
        freeSpots.push_back({x, y});
      }
    }
  }
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> distrib(0, freeSpots.size() - 1);
  game_data.apple = freeSpots[distrib(gen)];
}

/**
 * @brief Fills the game field with the current positions of the snake and the
 * apple.
 */
void snake_model::fillField() {
  for (int y = 0; y < 20; y++) {
    for (int x = 0; x < 10; x++) {
      game_info.field[y][x] = Field;
    }
  }
  for (Coord_t coord : game_data.snake) {
    game_info.field[coord.y][coord.x] = Body;
  }
  game_info.field[game_data.snake[0].y][game_data.snake[0].x] = Head;
  game_info.field[game_data.apple.y][game_data.apple.x] = Apple;
}

/**
 * @brief Handles user input and updates the game state accordingly.
 * @param action The user action to be processed.
 * @param hold Whether the action is being held.
 */
void snake_model::userInput(snake_model::UserAction_t action, bool hold) {
  if (game_data.isRotate) return;
  switch (action) {
    case snake_model::Left:
    case snake_model::Right:
      if ((game_data.head_dir == snake_model::Up ||
           game_data.head_dir == snake_model::Down) &&
          !game_info.pause /*&& !game_data.isRotate*/) {
        game_data.head_dir = action;
        game_data.isRotate = true;
      }
      break;
    case snake_model::Down:
    case snake_model::Up:
      if ((game_data.head_dir == snake_model::Left ||
           game_data.head_dir == snake_model::Right) &&
          !game_info.pause /* && !game_data.isRotate*/) {
        game_data.head_dir = action;
        game_data.isRotate = true;
      }
      break;
    case s21::snake_model::Start:
      if (game_info.pause == 1) {
        std::thread start_game = std::thread([&] { start(); });
        start_game.detach();
      } else if (game_info.pause == 2) {
        preStart();
      }
      break;
    case s21::snake_model::Pause:
      if (!game_info.pause) game_info.pause = 1;
      break;
    case s21::snake_model::Terminate:
      terminate();
      break;
    case s21::snake_model::Action:
      game_data.isSonic = hold;
      break;
  }
  //  action_mutex.unlock();
}

/**
 * @brief Returns the current speed of the game based on the current score.
 * @return The current speed of the game.
 */
int snake_model::getSpeed() {
  int s;
  game_info.level = game_info.score / 5;
  s = speed[game_info.level];
  if (game_data.isSonic) s -= 50;
  return s;
}
void snake_model::setApple(Coord_t apple) { game_data.apple = apple; }

// /**
//  * @brief Copies the game state from another snake_model object.
//  * @param s The snake_model object to copy the state from.
//  * @return A reference to the current snake_model object.
//  */
// snake_model& snake_model::operator=(snake_model const& s) {
//   if (this == &s) return *this;
//   game_info = s.game_info;
//   game_data = s.game_data;
//   game_info.field = new int*[20];
//   for (int y = 0; y < 20; y++) {
//     game_info.field[y] = new int[10];
//     std::memcpy(game_info.field[y], s.game_info.field[y], 10 * sizeof(int));
//   }
//   game_info.next = game_info.field;
//   return *this;
// }

/**
 * @brief Copy constructor for the snake_model class.
 * @param model The snake_model object to copy the state from.
 */
snake_model::snake_model(const snake_model& model) {
  game_data = model.game_data;
  game_data.snake = model.game_data.snake;
  game_info = model.game_info;
  game_info.field = new int*[20];
  for (int y = 0; y < 20; y++) {
    game_info.field[y] = new int[10];
    std::memcpy(game_info.field[y], model.game_info.field[y], 10 * sizeof(int));
  }
  game_info.next = nullptr;
}
