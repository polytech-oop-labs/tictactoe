/** This file defines game engine implementation. */

#pragma once
#include "game_state.h"
#include "player.h"
#include "composed_observer.h"


class GameEngine {
    const size_t MAX_SIZE_FOR_FIXED_FIELD = 8'192;

    class _View: public GameView {
        friend class GameEngine;
        GameEngine *_engine;
        _View(GameEngine *engine): _engine(engine) {}
    public:
        _View(_View&) = delete;
        const GameSettings& get_settings() const override
            { return _engine->_settings; }
        const GameState& get_state() const override
            { return _engine->_state; }
        void add_observer(Observer& observer) override 
            { _engine->_observers.add_observer(observer); }
        void remove_observer(Observer& observer) override
            { _engine->_observers.remove_observer(observer); }
    };

    GameState _state;
    GameSettings _settings;
    _View _view;

    ComposedObserver _observers;
    Player* _zero_player;
    Player* _cross_player;

    bool _is_move_correct(const Point& move, std::string& reason) const;
    bool _is_winning_move(const Point& move) const;

public:
    /** Instatiate new game with given settings. */
    GameEngine(const GameSettings& settings);
    /**
     * Instatiate game with given settings and field. This game can be already started.
     */
    GameEngine(const GameView& view);

    /**
     * Sets player for given `mark`.
     *
     * If `mark == Mark::None`, add player to play with first free mark. If two players
     * was already set, throws `std::overflow`.
     *
     * Add player as an observer in game and calls `assign_mark` for player.
     */
    void set_player(Player &p, Mark mark = Mark::None);
    /** Removes given player. */
    void remove_player(Player &p);
    /** Removes given player. */
    void remove_player(Mark player_mark);
    /** Returns player for given mark or `nullptr` if there is no such player. */
    Player* get_player(Mark player_mark);
    /** Returns player for given mark or `nullptr` if there is no such player. */
    const Player* get_player(Mark player_mark) const;

    /** Returns game view for the game. */
    GameView& get_view();
    /** Returns game view for the game. */
    const GameView& get_view() const;

    /**
     * Runs game. If `max_moves` is positive, runs at most `max_moves` moves.
     *
     * If game is finished, returns mark of player that has won or `Mark::None`
     * for draw. If game is not finished returns `Mark::None`.
     *
     * Throws `std::underflow_error` if players were not added. Throws
     * `std::logic_error` if game state is invalid.
     *
     */
    Mark play_game(size_t max_moves = 0);
};
