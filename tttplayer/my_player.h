#pragma once
#include <ostream>
#include "player.h"

/**
 * This is example player, that plays absolutely randomly.
 */
class RandomPlayer : public Player {
    std::string _name;
public:
    RandomPlayer(const std::string& name): _name(name) {}
    std::string get_name() const override { return _name; }
    Point play(const GameView& game) override;
    void assign_mark(Mark player_mark) override { /*does nothing*/ }
    void notify(const GameView&, const Event&) override { /*does nothing*/ }
};


/**
 * Simple observer, that logs every event in given output stream.
 */
class BasicObserver : public Observer {
    std::ostream& _out;

    std::ostream& _print_mark(Mark mark);
public:
    BasicObserver(std::ostream& out_stream): _out(out_stream) {}
    void notify(const GameView&, const Event& event) override;
};
