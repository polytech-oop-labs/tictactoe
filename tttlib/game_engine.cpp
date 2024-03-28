#include "game_engine.h"

#include <stdexcept>

#include "basic_fields.h"


GameEngine::GameEngine(const GameSettings& settings):
    _settings(settings), 
    _view(this), 
    _zero_player(0), 
    _cross_player(0)
{
    const size_t n_cells = settings.field_size.get_width() 
        * settings.field_size.get_height();
    if (n_cells <= MAX_SIZE_FOR_FIXED_FIELD) {
        _state.field = std::make_unique<FixedSizeField>(settings.field_size);
    }
    else {
        _state.field = std::make_unique<InfiniteField>();
    }
    _state.number_of_moves = 0;
    _state.current_move = Mark::Cross;
    _state.is_finished = false;
    if (_settings.max_moves > n_cells || _settings.max_moves == 0) {
        _settings.max_moves = n_cells;
    }
}

GameEngine::GameEngine(const GameView& view): 
    GameEngine(view.get_settings()) 
{
    auto & other_state = view.get_state();
    for (auto it = other_state.field->get_iterator(); it->has_value(); it->step()) {
        _state.field->set_value(it->get_point(), it->get_value());
    }
    _state.number_of_moves = other_state.number_of_moves;
    _state.current_move = other_state.current_move;
    _state.is_finished = other_state.is_finished;
}

void GameEngine::set_player(Player &p, Mark mark) {
    if (mark == Mark::None) {
        if (!_cross_player) mark = Mark::Cross;
        else if (!_zero_player) mark = Mark::Zero;
        else throw std::overflow_error("Cannot add another player");
    }
    p.assign_mark(mark);
    _view.add_observer(p);
    _observers.notify(_view, PlayerJoinedEvent::make(p.get_name(), mark));
    (mark == Mark::Cross ? _cross_player : _zero_player) = &p;
}

void GameEngine::remove_player(Player &p) {
    if (_zero_player == &p) _zero_player = 0;
    if (_cross_player == &p) _cross_player = 0;
}

void GameEngine::remove_player(Mark player) {
    if (auto ptr = get_player(player)) remove_player(*ptr);
}

Player* GameEngine::get_player(Mark player) {
    switch (player) {
        case Mark::Zero: return _zero_player;
        case Mark::Cross: return _cross_player;
        case Mark::None: return 0;
    }
    return 0;
}

const Player* GameEngine::get_player(Mark player) const {
    switch (player) {
        case Mark::Zero: return _zero_player;
        case Mark::Cross: return _cross_player;
        case Mark::None: return 0;
    }
    return 0;
}

GameView& GameEngine::get_view() { return _view; }
const GameView& GameEngine::get_view() const { return _view; }

Mark GameEngine::play_game(size_t max_moves) {
    if (_state.is_finished) return _state.current_move;

    if (_state.current_move == Mark::None) {
        throw std::logic_error("State is invalid");
    }

    if (!_zero_player || !_cross_player) {
        throw std::underflow_error("Not enough players to play game");
    }

    if (_state.number_of_moves == 0) {
        _observers.notify(_view, GameStartedEvent::make());
    }

    std::string disqualification_reason;
    for (size_t i = 0; 
            max_moves == 0 || i < max_moves; 
            ++i, ++_state.number_of_moves) {

        if (_state.number_of_moves >= _settings.max_moves) {
            _observers.notify(_view, DrawEvent::make("Max moves limit reached"));
            _state.is_finished = true;
            _state.current_move = Mark::None;
            break;
        }

        Player& player_to_move = _state.current_move == Mark::Cross 
            ? *_cross_player 
            : *_zero_player;

        const Mark other_mark = _state.current_move == Mark::Cross 
            ? Mark::Zero 
            : Mark::Cross;

        Point move;

        try {
            move = player_to_move.play(_view);
        } 
        catch (const PlayerMoveException& exp) {
            disqualification_reason = exp.reason;
        }
        catch (...) {
            disqualification_reason = "exception is thrown while making player move";
        }

        if (!disqualification_reason.empty() || !_is_move_correct(move, disqualification_reason)) {
            _observers.notify(_view, DisqualificationEvent::make(
                        disqualification_reason, _state.current_move));
            _state.is_finished = true;
            _state.current_move = other_mark;
            break;
        }

        _state.field->set_value(move, _state.current_move);
        _observers.notify(_view, MoveEvent::make(move, _state.current_move));

        if (_is_winning_move(move)) {
            _observers.notify(_view, WinEvent::make(_state.current_move));
            _state.is_finished = true;
            break;
        }

        _state.current_move = other_mark;
    }
    return _state.current_move;
}

bool GameEngine::_is_move_correct(const Point& move, std::string& reason) const {
    if (!_settings.field_size.is_within(move)) {
        reason = "Move is outside of field";
        return false;
    }
    if (_state.field->get_value(move) != Mark::None) {
        reason = "Cell was already taken";
        return false;
    }
    return true;
}

bool GameEngine::_is_winning_move(const Point& move) const {
    const Mark mark = _state.field->get_value(move);
    auto bounds = _state.field->get_current_boundary();
    for (int dx = 0; dx <= 1; ++dx) {
        for (int dy = -1; dy <= 1; ++dy) {
            if (dx == 0 && (dy == 0 || dy == -1)) continue;
            int max_forward = 0, max_backward = 0;
            Point p;

            p = move;
            do {
                p.x += dx;
                p.y += dy;
                ++max_forward;
            } while(max_forward <= _settings.win_length
                    && bounds.is_within(p) 
                    && _state.field->get_value(p) == mark);

            p = move;
            do {
                p.x -= dx;
                p.y -= dy;
                ++max_backward;
            } while(max_backward <= _settings.win_length - max_forward + 1
                    && bounds.is_within(p) 
                    && _state.field->get_value(p) == mark);

            if (max_forward + max_backward - 1 >= _settings.win_length) {
                return true;
            }
        }
    }
    return false;
}
