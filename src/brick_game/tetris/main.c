#include "../../gui/cli/cli.h"

int main(int argc, char **argv) {
  GameInfo_t *game = create_game(HEIGHT + 2, WIDTH);
  UserAction_t movement = Pause;
  bool game_over = true;
  print_logo();
  system("stty cbreak -echo");
  getchar();
  system("stty cooked echo");
  WINDOW *field, *next, *scoreboard, *help;

  if (argc >= 2) {
    FILE *file = fopen(argv[1], "r");
    if (file == NULL) {
      clean_memory(game);
      perror("FILE NOT FOUND");
      exit(EXIT_FAILURE);
    } else {
      fscanf(file, "%d", &game->high_score);
      fclose(file);
    }
  }

  initscr();
  cbreak();
  noecho();
  keypad(stdscr, true);
  timeout(0);
  curs_set(0);

  change_colors();

  field = newwin(HEIGHT + 2, 2 * WIDTH + 2, 0, 22);
  next = newwin(8, 16, 0, 2 * (WIDTH + 1) + 22);
  help = newwin(16, 16, 8, 2 * (WIDTH + 1) + 22);
  scoreboard = newwin(8, 22, 0, 0);

  while (game_over) {
    game_over = tick(game, movement);
    display_field(field, game);
    display_next(next, game->next_tetromino);
    display_statistics(scoreboard, game);
    display_help(help);
    doupdate();
    sleep_milli(10);
    switch (getch()) {
      case KEY_LEFT:
        movement = Left;
        break;
      case KEY_RIGHT:
        movement = Right;
        break;
      case ' ':
        movement = Action;
        break;
      case KEY_DOWN:
        movement = Down;
        break;
      case 'q':
        game_over = false;
        movement = Terminate;
        break;
      case 'p':
        wclear(field);
        box(field, 0, 0);
        wmove(field, HEIGHT / 2, (WIDTH) / 2 + 4);
        wprintw(field, "PAUSE");
        wrefresh(field);
        timeout(-1);
        getch();
        timeout(0);
        movement = Pause;
        break;
      default:
        movement = Start;
    }
    game->high_score = max(game->score, game->high_score);
    // wrefresh(field);
    // wrefresh(next);
    // wrefresh(help);
    // wrefresh(scoreboard);
  }

  wclear(stdscr);
  delwin(field);
  delwin(next);
  delwin(scoreboard);
  endwin();
  system("clear");

  if (argc >= 2) {
    FILE *file = fopen(argv[1], "w");
    if (file == NULL) {
      clean_memory(game);
      perror("FILE NOT FOUND");
      exit(EXIT_FAILURE);
    } else {
      fprintf(file, "%d", game->high_score);
      fclose(file);
    }
  }

  print_game_over(game->score);
  clean_memory(game);

  return 0;
}