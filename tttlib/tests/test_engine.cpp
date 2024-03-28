#include "basic_fields.h"
#include "tests_common.h"
#include "game_engine.h"

#include <vector>


class StubPlayer: public Player {
    Point _move;
    std::string _name;

public:
    StubPlayer(const Point& move, std::string name = "stub_player"): 
        _move(move), _name(name) {}
    std::string get_name() const override { return _name; }
    Point play(const GameView& game) override { return _move; }
    void assign_mark(Mark) override {}
    void notify(const GameView&, const Event&) override {}

    void set_next_move(const Point& move) { _move = move; }
};


class SimpleObserver: public Observer {
    std::vector<Event> _events;
public:
    void notify(const GameView&, const Event& event) override 
    { _events.push_back(event); }

    const Event& get_event(size_t i) { return _events.at(i); }
};


struct BasicEventsTest {
    std::string name = "basic_events";

    int operator()() {
        Boundary field_size = {.min = {.x = -1, .y = -1}, .max = {.x = 1, .y = 1}};
        GameSettings settings = {
            .field_size = field_size, 
            .max_moves = 0, 
            .win_length = 3,
            .move_timelimit_ms = 0,
        };
        GameEngine game(settings);
        ASSERT(game.get_view().get_settings().max_moves == 9);
        SimpleObserver obs;
        game.get_view().add_observer(obs);
        Point p1 = {.x = 0, .y = 1}, p2 = {.x = 1, .y = 3};
        StubPlayer player1(p1, "a"), player2(p2, "b");
        game.set_player(player1);
        game.set_player(player2);
        {
            const Event& ev = obs.get_event(0);
            ASSERT(ev.get_type() == PlayerJoinedEvent::TYPE);
            auto& data = get_data<PlayerJoinedEvent>(ev);
            ASSERT(data.mark == Mark::Cross);
            ASSERT(data.name == "a");
        }
        {
            const Event& ev = obs.get_event(1);
            ASSERT(ev.get_type() == PlayerJoinedEvent::TYPE);
            auto& data = get_data<PlayerJoinedEvent>(ev);
            ASSERT(data.mark == Mark::Zero);
            ASSERT(data.name == "b");
        }
        Mark result = game.play_game(0);
        {
            const Event& ev = obs.get_event(2);
            ASSERT(ev.get_type() == GameStartedEvent::TYPE);
        }
        {
            const Event& ev = obs.get_event(3);
            ASSERT(ev.get_type() == MoveEvent::TYPE);
            auto& data = get_data<MoveEvent>(ev);
            ASSERT(data.mark == Mark::Cross);
            ASSERT(data.point == p1);
        }
        {
            const Event& ev = obs.get_event(4);
            ASSERT(ev.get_type() == DisqualificationEvent::TYPE);
            auto& data = get_data<DisqualificationEvent>(ev);
            ASSERT(data.player_mark == Mark::Zero);
        }
        ASSERT(result == Mark::Cross);
        return 0;
    }
};


class ViewStub: public GameView {
    GameState& state;
    GameSettings& settings;

public:
    ViewStub(GameState& state, GameSettings& settings):
        state(state), settings(settings) {}
    const GameSettings& get_settings() const { return settings; }
    const GameState& get_state() const { return state; }

    void add_observer(Observer& observer) {}
    void remove_observer(Observer& observer) {}
};


GameEngine make_from_field(const char str[9]) {
    Boundary field_size = {
        .min = {.x = -1, .y = -1}, 
        .max = {.x = 1, .y = 1}
    };
    GameSettings settings = {
        .field_size = field_size, 
        .max_moves = 0, 
        .win_length = 3,
        .move_timelimit_ms = 0,
    };
    FixedSizeField field(field_size);
    size_t count = 0;
    for (int i = 0; i < 9; ++i) {
        Point p = {.x = i % 3 - 1, .y =  -(i / 3 - 1)};
        ++count;
        switch (str[i]) {
            case 'x': field.set_value(p, Mark::Cross); break;
            case 'o': field.set_value(p, Mark::Zero); break;
            default: --count; break;
        }
    }
    GameState state = {
        .field = std::make_unique<FixedSizeField>(field),
        .number_of_moves = count,
        .current_move = Mark::Cross,
        .is_finished = false,
    };
    return GameEngine(ViewStub(state, settings));
}

struct TestWinningMoves {
    std::string name = "winning_moves";
    bool is_winning(const char field[9], Point move) {
        GameEngine game = make_from_field(field);
        StubPlayer p1(move), p2(Point({.x = 0, .y = 0}));
        game.set_player(p1);
        game.set_player(p2);
        Mark result = game.play_game(1);
        return result == Mark::Cross;
    }
    
    int operator()() {
        ASSERT(is_winning(
                    "xx." 
                    "o.o" 
                    "xo.", 
                    Point({.x = 1, .y = 1})));
        ASSERT(!is_winning(
                    "xx." 
                    "o.o" 
                    "xo.", 
                    Point({.x = 0, .y = 0})));
        ASSERT(is_winning(
                    "xx." 
                    "o.o" 
                    "xx.", 
                    Point({.x = 0, .y = 0})));
        ASSERT(is_winning(
                    "xx." 
                    "o.o" 
                    "xx.", 
                    Point({.x = 1, .y = -1})));
        ASSERT(!is_winning(
                    "xx." 
                    "o.o" 
                    "xo.", 
                    Point({.x = 1, .y = -1})));
        return 0;
    }
};

struct TestDraw {
    std::string name = "draw";
    int operator()() {
        GameEngine game = make_from_field("xxo" "oo." "xox");
        StubPlayer p1(Point({.x = 1, .y = 0})), p2(Point({.x = 0, .y = 0}));
        game.set_player(p1);
        game.set_player(p2);
        SimpleObserver obs;
        game.get_view().add_observer(obs);
        Mark mark = game.play_game(0);
        ASSERT(mark == Mark::None);
        const Event& ev = obs.get_event(1);
        ASSERT(ev.get_type() == DrawEvent::TYPE);
        return 0;
    }
};


TESTS(BasicEventsTest, TestWinningMoves, TestDraw)
