#pragma once

#include "field.h"
#include "events.h"

struct GameSettings {
    /** Boundary of field, all moves must be done within this boundary */
    Boundary field_size;
    /**
     * Maximum number of moves, if game does not end before this many moves,
     * then game will end with draw
     */
    size_t max_moves;
    /** Size of line, that player must fill to win the game */
    size_t win_length;
    /**
     * Timelimit in milliseconds for each player to make a move in milliseconds.
     *
     * This option will be ignored when running engine locally, but this option
     * is needed for remote game.
     */
    size_t move_timelimit_ms;
};

/** State of the game, that will mutate during game process */
struct GameState {
    std::unique_ptr<Field> field;
    /**
     * Number of moves that has already happened in game, equals to zero if game
     * has not started.
     */
    size_t number_of_moves;
    /**
     * If game is not finished, then `current_move` will be the mark of player,
     * that will move next.
     *
     * If game is finished, then `current_move` will be the mark of winner
     * (`Mark::None` for draw).
     *
     */
    Mark current_move;
    bool is_finished;
};

class Observer;

/**
 * Read only view of game. One can add an observer to receive updates of game.
 */
class GameView {
public:
    virtual ~GameView() {}
    virtual const GameSettings& get_settings() const = 0;
    virtual const GameState& get_state() const = 0;

    virtual void add_observer(Observer& observer) = 0;
    virtual void remove_observer(Observer& observer) = 0;
};

/**
 * Observer of `GameView` that will handle events and state changes of
 * game.
 */
class Observer {
public:
    virtual ~Observer() {}
    /** This method will be called when an event happens in `game`. */
    virtual void notify(const GameView& game, const Event& event) = 0;
};
