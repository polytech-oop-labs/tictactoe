#include "events.h"

const std::string MoveEvent::TYPE = "move";

MoveEvent::MoveEvent(const Point& point, Mark mark):
    point(point), mark(mark) {}

Event MoveEvent::make(const Point &point, Mark mark) {
    return Event(TYPE, new MoveEvent(point, mark));
}

const std::string PlayerJoinedEvent::TYPE = "player_joined";

PlayerJoinedEvent::PlayerJoinedEvent(const std::string& name, Mark mark):
    name(name), mark(mark) {}

Event PlayerJoinedEvent::make(const std::string &name, Mark mark) {
    return Event(TYPE, new PlayerJoinedEvent(name, mark));
}

const std::string GameStartedEvent::TYPE = "game_started";

Event GameStartedEvent::make() {
    return Event(TYPE, new GameStartedEvent());
}

const std::string WinEvent::TYPE = "win";

WinEvent::WinEvent(Mark winner_mark):
    winner_mark(winner_mark) {}

Event WinEvent::make(Mark winner_mark) {
    return Event(TYPE, new WinEvent(winner_mark));
}

const std::string DrawEvent::TYPE = "draw";

DrawEvent::DrawEvent(const std::string &reason):
    reason(reason) {}

Event DrawEvent::make(const std::string &reason) {
    return Event(TYPE, new DrawEvent(reason));
}

const std::string DisqualificationEvent::TYPE = "disqualification";

DisqualificationEvent::DisqualificationEvent(const std::string &reason, 
        Mark player_mark): reason(reason), player_mark(player_mark) {}

Event DisqualificationEvent::make(const std::string &reason, Mark player_mark) {
    return Event(TYPE, new DisqualificationEvent(reason, player_mark));
}

const std::string MessageEvent::TYPE = "message";

MessageEvent::MessageEvent(const std::string &source, const std::string &message):
    source(source), message(message) {}

Event MessageEvent::make(const std::string &source, const std::string &message) {
    return Event(TYPE, new MessageEvent(source, message));
}
