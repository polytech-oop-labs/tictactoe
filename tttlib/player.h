#pragma once
#include <string>
#include <exception>

#include "game_state.h"


/**
 * This exception is thrown by player if it cannot make a move.
 */
struct PlayerMoveException: public std::exception {
    std::string reason;
};

/**
 * Base class for player in game. Every player is a game observer and 
 * can react to game events.
 */
class Player: public Observer {
public:
    virtual ~Player() {}
    virtual std::string get_name() const = 0;
    /**
     * This method will be called when player joins game to tell player,
     * whether he/she plays with Zeros or Crosses.
     */
    virtual void assign_mark(Mark player_mark) = 0;
    /**
     * This method will be called when player must make move. Method returns
     * point, where player will place his/her mark.
     *
     * If move is invalid (if it is placed out of field or return cell is not
     * empty) player loses immediately by disqualification.
     *
     * Throws `PlayerMoveException` if something goes wrong.
     */
    virtual Point play(const GameView& game) = 0;
};
