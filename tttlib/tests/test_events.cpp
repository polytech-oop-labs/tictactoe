#include "tests_common.h"
#include "events.h"


struct MakeMoveEventTest {
    std::string name = "move_event";
    int operator()() {
        Event ev = MoveEvent::make({.x = 1, .y = 2}, Mark::Zero);
        ASSERT(ev.get_type() == MoveEvent::TYPE);
        auto& data = get_data<MoveEvent>(ev);
        ASSERT(data.mark == Mark::Zero && data.point.x == 1 && data.point.y == 2);
        return 0;
    }
};

struct MakeMessageEventTest {
    std::string name = "message_event";
    int operator()() {
        std::string source = "s", message = "m";
        Event ev = MessageEvent::make(source, message);
        ASSERT(ev.get_type() == MessageEvent::TYPE);
        auto& data = get_data<MessageEvent>(ev);
        ASSERT(data.message == message && data.source == source);
        return 0;
    }
};

struct BadCastTest {
    std::string name = "bad_cast";
    int operator()() {
        Event ev = GameStartedEvent::make();
        try {
            (void) get_data<MoveEvent>(ev);
        }
        catch (std::bad_cast) {
            return 0;
        }
        catch (...) {
            return 1;
        }
        return 0;
    }
};

TESTS(MakeMoveEventTest, MakeMessageEventTest, BadCastTest)
