#include "tetris.h"

position TETRIS_BRICK[NUMBER_OF_TETROMINOS][NUMBER_OF_ORIENTATIONS]
                     [NUMBER_OF_CELLS] = {
                         /* I */
                         {{{2, 0}, {2, 1}, {2, 2}, {2, 3}},
                          {{0, 1}, {1, 1}, {2, 1}, {3, 1}},
                          {{2, 0}, {2, 1}, {2, 2}, {2, 3}},
                          {{0, 1}, {1, 1}, {2, 1}, {3, 1}}},
                         /* O */
                         {{{1, 1}, {1, 2}, {2, 1}, {2, 2}},
                          {{1, 1}, {1, 2}, {2, 1}, {2, 2}},
                          {{1, 1}, {1, 2}, {2, 1}, {2, 2}},
                          {{1, 1}, {1, 2}, {2, 1}, {2, 2}}},
                         /* T */
                         {{{0, 1}, {1, 0}, {1, 1}, {2, 1}},
                          {{0, 1}, {1, 0}, {1, 1}, {1, 2}},
                          {{0, 1}, {1, 1}, {1, 2}, {2, 1}},
                          {{1, 0}, {1, 1}, {1, 2}, {2, 1}}},
                         /* J */
                         {{{2, 0}, {2, 1}, {2, 2}, {1, 2}},
                          {{1, 1}, {1, 2}, {2, 2}, {3, 2}},
                          {{1, 0}, {2, 0}, {1, 1}, {1, 2}},
                          {{1, 1}, {2, 1}, {3, 1}, {3, 2}}},
                         /* L */
                         {{{1, 0}, {1, 1}, {1, 2}, {2, 2}},
                          {{1, 1}, {1, 2}, {2, 1}, {3, 1}},
                          {{1, 0}, {2, 0}, {2, 1}, {2, 2}},
                          {{2, 1}, {2, 2}, {1, 2}, {0, 2}}},
                         /* S */
                         {{{2, 1}, {3, 1}, {1, 2}, {2, 2}},
                          {{1, 0}, {1, 1}, {2, 1}, {2, 2}},
                          {{2, 1}, {3, 1}, {1, 2}, {2, 2}},
                          {{1, 0}, {1, 1}, {2, 1}, {2, 2}}},
                         /* Z */
                         {{{1, 1}, {2, 1}, {2, 2}, {3, 2}},
                          {{1, 1}, {2, 1}, {2, 0}, {1, 2}},
                          {{1, 1}, {2, 1}, {2, 2}, {3, 2}},
                          {{1, 1}, {2, 1}, {2, 0}, {1, 2}}}};

int LEVEL_TICKRATE[MAX_LEVEL + 1] = {50, 45, 40, 35, 30, 25, 20, 15, 10, 5, 1};

char get_cell(GameInfo_t *game, int row, int col) {
  return game->field[col][row];
}

void set_cell(GameInfo_t *game, int row, int col, char val) {
  game->field[col][row] = val;
}

bool is_cell_valid(int row, int col) {
  return ((row >= 0) && (row < HEIGHT) && (col >= 0) && (col < WIDTH)) ? 1 : 0;
}

void place_tetromino(GameInfo_t *game, tetromino tetromino_to_put) {
  for (int i = 0; i < NUMBER_OF_CELLS; ++i) {
    position cell_position =
        TETRIS_BRICK[tetromino_to_put.type][tetromino_to_put.orientation][i];
    set_cell(game, tetromino_to_put.pos.y + cell_position.y,
             tetromino_to_put.pos.x + cell_position.x,
             tetromino_to_put.type + 1);
  }
}

void remove_tetromino(GameInfo_t *game, tetromino tetromino_to_remove) {
  for (int i = 0; i < NUMBER_OF_CELLS; ++i) {
    position cell_position = TETRIS_BRICK[tetromino_to_remove.type]
                                         [tetromino_to_remove.orientation][i];
    set_cell(game, tetromino_to_remove.pos.y + cell_position.y,
             tetromino_to_remove.pos.x + cell_position.x, EMPTY_CELL);
  }
}

int random_tetromino(void) { return rand() % NUMBER_OF_TETROMINOS; }

bool is_tetromino_valid(GameInfo_t *game, tetromino tetromino_to_check) {
  for (int i = 0; i < NUMBER_OF_CELLS; ++i) {
    position cell_position = TETRIS_BRICK[tetromino_to_check.type]
                                         [tetromino_to_check.orientation][i];
    int x = tetromino_to_check.pos.x + cell_position.x;
    int y = tetromino_to_check.pos.y + cell_position.y;
    if (!is_cell_valid(y, x) || get_cell(game, y, x) != EMPTY_CELL) {
      return false;
    }
  }
  return true;
}

void generate_new_tetromino(GameInfo_t *game) {
  game->current_tetromino = game->next_tetromino;
  game->next_tetromino.type = random_tetromino();
  game->next_tetromino.orientation = 0;
  game->next_tetromino.pos.y = 0;
  game->next_tetromino.pos.x = WIDTH / 2 - 2;
}

void move_tetromino_horizontal(GameInfo_t *game, int direction) {
  remove_tetromino(game, game->current_tetromino);
  game->current_tetromino.pos.x += direction;
  if (!is_tetromino_valid(game, game->current_tetromino)) {
    game->current_tetromino.pos.x -= direction;
  }
  place_tetromino(game, game->current_tetromino);
}

void rotate_tetromino(GameInfo_t *game, int rotation) {
  remove_tetromino(game, game->current_tetromino);
  game->current_tetromino.orientation += rotation;
  while (true) {
    game->current_tetromino.orientation =
        (game->current_tetromino.orientation) % NUMBER_OF_ORIENTATIONS;

    if (is_tetromino_valid(game, game->current_tetromino)) {
      break;
    }
    game->current_tetromino.pos.x--;
    if (is_tetromino_valid(game, game->current_tetromino)) {
      break;
    }
    game->current_tetromino.pos.x += 2;

    if (is_tetromino_valid(game, game->current_tetromino)) {
      break;
    }
    game->current_tetromino.pos.x--;
    game->current_tetromino.orientation -= rotation;
    game->current_tetromino.orientation =
        (game->current_tetromino.orientation) % NUMBER_OF_ORIENTATIONS;
    break;
  }
  place_tetromino(game, game->current_tetromino);
}

void drop_tetromino_down(GameInfo_t *game) {
  remove_tetromino(game, game->current_tetromino);
  while (is_tetromino_valid(game, game->current_tetromino)) {
    game->current_tetromino.pos.y++;
  }
  game->current_tetromino.pos.y--;
  place_tetromino(game, game->current_tetromino);
  generate_new_tetromino(game);
}

bool is_line_full(GameInfo_t *game, int line_number) {
  for (int i = 0; i < WIDTH; ++i) {
    if (get_cell(game, line_number, i) == EMPTY_CELL) {
      return false;
    }
  }
  return true;
}

void shift_lines(GameInfo_t *game, int line_number) {
  for (int i = line_number - 1; i >= 0; --i) {
    for (int j = 0; j < WIDTH; ++j) {
      set_cell(game, i + 1, j, get_cell(game, i, j));
      set_cell(game, i, j, EMPTY_CELL);
    }
  }
}

int check_for_removals(GameInfo_t *game) {
  int lines_cleared = 0;
  remove_tetromino(game, game->current_tetromino);
  for (int i = HEIGHT - 1; i >= 0; --i) {
    if (is_line_full(game, i)) {
      shift_lines(game, i);
      i++;
      lines_cleared++;
    }
  }
  place_tetromino(game, game->current_tetromino);
  return lines_cleared;
}

void update_score(GameInfo_t *game, int lines_cleared) {
  int score_multiplier[] = {0, 100, 300, 700, 1500};
  game->score += (score_multiplier[lines_cleared] * (game->level + 1));
  game->points_remain += (score_multiplier[lines_cleared] * (game->level + 1));
  if (game->points_remain >= POINTS_PER_LEVEL) {
    game->level = min(MAX_LEVEL, game->level + 1);
    game->points_remain -= POINTS_PER_LEVEL;
  }
}

bool is_game_over(GameInfo_t *game) {
  bool game_over = false;
  remove_tetromino(game, game->current_tetromino);
  for (int i = 0; i < 2; ++i) {
    for (int j = 0; j < WIDTH; ++j) {
      if (get_cell(game, i, j) != EMPTY_CELL) {
        game_over = true;
      }
    }
  }
  place_tetromino(game, game->current_tetromino);
  return game_over;
}

void make_movement(GameInfo_t *game, UserAction_t movement) {
  switch (movement) {
    case Left:
      move_tetromino_horizontal(game, -1);
      break;
    case Right:
      move_tetromino_horizontal(game, 1);
      break;
    case Action:
      rotate_tetromino(game, 1);
      break;
    case Down:
      drop_tetromino_down(game);
      break;
    default:
      break;
  }
}

void userInput(UserAction_t action, bool hold) {
  (void)action;
  (void)hold;
  updateCurrentState();
}

GameInfo_t updateCurrentState() {
  GameInfo_t game = {0};
  return game;
}

void make_a_step(GameInfo_t *game) {
  game->speed--;
  if (game->speed <= 0) {
    remove_tetromino(game, game->current_tetromino);
    game->current_tetromino.pos.y++;
    if (is_tetromino_valid(game, game->current_tetromino)) {
      game->speed = LEVEL_TICKRATE[game->level];
    } else {
      game->current_tetromino.pos.y--;
      place_tetromino(game, game->current_tetromino);
      generate_new_tetromino(game);
    }
    place_tetromino(game, game->current_tetromino);
  }
}

bool tick(GameInfo_t *game, UserAction_t movement) {
  make_a_step(game);
  make_movement(game, movement);
  int lines_removed = check_for_removals(game);
  update_score(game, lines_removed);
  return !is_game_over(game);
}

void init_game(GameInfo_t *game, int field_height, int field_width) {
  game->field = malloc(field_width * field_height * sizeof(char) +
                       field_width * sizeof(char *));
  char *ptr = (char *)(game->field + field_width);
  for (int i = 0; i < field_width; ++i) {
    game->field[i] = ptr + field_height * i;
  }
  for (int i = 0; i < field_height; ++i) {
    for (int j = 0; j < field_width; ++j) {
      game->field[j][i] = EMPTY_CELL;
    }
  }
  game->level = 0;
  game->score = 0;
  game->high_score = 0;
  game->speed = LEVEL_TICKRATE[game->level];
  game->points_remain = 0;
  srand(time(NULL));
  generate_new_tetromino(game);
  generate_new_tetromino(game);
  game->next_tetromino.pos.x = WIDTH / 2 - 2;
}

GameInfo_t *create_game(int height, int width) {
  GameInfo_t *game = malloc(sizeof(GameInfo_t));
  init_game(game, height, width);
  return game;
}

void clean_memory(GameInfo_t *game) {
  free(game->field);
  free(game);
}