#include <cstdlib>
#include <ctime>
#include <iostream>

#include "my_player.h"
#include "game_engine.h"


int main() {
    srand(time(NULL));
    BasicObserver obs(std::cout);
    GameSettings settings = {
        .field_size = {
            .min = {.x = -2, .y = -2},
            .max = {.x = 2, .y = 2},
        },
        .max_moves = 0,
        .win_length = 4,
        .move_timelimit_ms = 0,
    };
    GameEngine game(settings);
    game.get_view().add_observer(obs);
    RandomPlayer player1("Vasya");
    game.set_player(player1);
    RandomPlayer player2("Kolya");
    game.set_player(player2);
    game.play_game();
    return 0;
}
