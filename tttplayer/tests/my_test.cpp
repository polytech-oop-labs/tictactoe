#include "my_player.h"
#include "game_engine.h"


int main() {
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
    RandomPlayer player1("Vasya");
    game.set_player(player1);
    RandomPlayer player2("Kolya");
    game.set_player(player2);
    game.play_game();

    // somehow test, that your player is playing properly
    // if it is not, return 1

    return 0;
}
