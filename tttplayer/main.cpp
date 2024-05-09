/**
 *  This is example of game, that you can use for debugging.
 */
#include <cstdlib>
#include <ctime>
#include <curses.h>
#include <iostream>
#include <ncurses.h>

#include "game_engine.h"
#include "my_player.h"
#include "ncurses_observer.h"

class Visualizer {

public:
  Visualizer() { initscr(); }
  ~Visualizer() { endwin(); }
};

int main() {

  Visualizer visualizer;
  Boundary field_size = {
      .min = {.x = -20, .y = -20},
      .max = {.x = 20, .y = 20},
  };

  FieldWindow win(newwin(42, 42, 1, 1), field_size);
  refresh();

  NCursesObserver obs(win);
  srand(time(NULL));

  /// Create setting for game
  GameSettings settings = {
      .field_size = field_size,
      .max_moves = 0,
      .win_length = 4,
      .move_timelimit_ms = 0,
  };

  /// Creates game engine with given settings
  GameEngine game(settings);

  /// Adds observer to the view of this game engine
  game.get_view().add_observer(obs);
  /// Creates first player
  RandomPlayer player1("Vasya");
  /// Adds it to play Xs
  game.set_player(player1);
  /// Create second player
  RandomPlayer player2("Kolya");
  /// Adds it to play Os
  game.set_player(player2);
  /// Starts game until someone wins.
  game.play_game();
  getch();
  endwin();
  return 0;
}
