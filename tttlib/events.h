/**
 * This file defines basic game events.
 *
 * Event is a tagged union with string tag and dynamicly allocated data.
 *
 * Each event type has unique string id and data structure derived from
 * `EventData`. It is assumed, that any subclass `EventData` will have
 * static `TYPE` field with unique id of this type of event and static `make`
 * function, that constructs `Event` object with corresponding id.
 */

#pragma once

#include <string>
#include <memory>
#include <typeinfo>

#include "field.h"


/**
 * Type for event class id
 */
using event_type_t = std::string;


/**
 *  Base class for all event data
 */
struct EventData {
    virtual ~EventData() {}
    virtual std::unique_ptr<EventData> copy() const = 0;
};


/**
 * Object that is representing event.
 *
 * Type of event has to be corresponded to `TYPE` field of 
 * corresponding event type.
 */
class Event {
    event_type_t _type;
    std::unique_ptr<EventData> _data;
public:
    Event(const event_type_t& type, EventData *data): 
        _type(type), _data(data) {}
    Event(const Event& other):
        _type(other._type), _data(other._data->copy()) {}
    const event_type_t& get_type() const { return _type; }
    const EventData& get_data() const { return *_data; }
};

/**
 * This event happens after player, that is playing with `mark` makes a valid
 * move by placing `mark` at `point`.
 */
struct MoveEvent: public EventData {
    static const event_type_t TYPE;

    Point point;
    Mark mark;

    MoveEvent(const Point& point, Mark mark);
    static Event make(const Point& point, Mark mark);
    std::unique_ptr<EventData> copy() const 
    { return std::unique_ptr<EventData>(new MoveEvent(*this)); }
};

/**
 * This event happens after player with name `name` joined game and he is about
 * to play with `mark`.
 */
struct PlayerJoinedEvent: public EventData {
    static const event_type_t TYPE;

    std::string name;
    Mark mark;

    PlayerJoinedEvent(const std::string& name, Mark mark);
    static Event make(const std::string& name, Mark mark);
    std::unique_ptr<EventData> copy() const 
    { return std::unique_ptr<EventData>(new PlayerJoinedEvent(*this)); }
};

/**
 * This event happens when game is started.
 */
struct GameStartedEvent: public EventData {
    static const event_type_t TYPE;

    static Event make();
    std::unique_ptr<EventData> copy() const 
    { return std::unique_ptr<EventData>(new GameStartedEvent(*this)); }
};

/**
 * This event happens when player that plays with `mark` wins the game.
 */
struct WinEvent: public EventData {
    static const event_type_t TYPE;

    Mark winner_mark;

    WinEvent(Mark winner_mark);
    static Event make(Mark winner_mark);
    std::unique_ptr<EventData> copy() const 
    { return std::unique_ptr<EventData>(new WinEvent(*this)); }
};

/**
 * This event happens when draw happens with given `reason`.
 */
struct DrawEvent: public EventData {
    static const event_type_t TYPE;

    std::string reason;

    DrawEvent(const std::string& reason);
    static Event make(const std::string& reason);
    std::unique_ptr<EventData> copy() const 
    { return std::unique_ptr<EventData>(new DrawEvent(*this)); }
};

/**
 * This event happens when player, that is playing `player_mark`, makes invalid
 * move that leads to disqualification of that player.
 */
struct DisqualificationEvent: public EventData {
    static const event_type_t TYPE;

    std::string reason;
    Mark player_mark;

    DisqualificationEvent(const std::string& reason, Mark player_mark);
    static Event make(const std::string& reason, Mark player_mark);
    std::unique_ptr<EventData> copy() const 
    { return std::unique_ptr<EventData>(new DisqualificationEvent(*this)); }
};

/**
 * This event happens when message is sent from `source` with given `message` text.
 */
struct MessageEvent: public EventData {
    static const event_type_t TYPE;

    std::string source;
    std::string message;

    MessageEvent(const std::string& source, const std::string& message);
    static Event make(const std::string& source, const std::string& message);
    std::unique_ptr<EventData> copy() const 
    { return std::unique_ptr<EventData>(new MessageEvent(*this)); }
};

/**
 * This is helper function, that casts data of given event to type of data of
 * this event. This function must be used only if you are sure, that type of 
 * event matches `EventDataType`, if it does not `std::bad_cast` will be thrown.
 *
 * Example: 
 * 
 * ```c++
 * void foo(const Event& ev) {
 *   if (ev.get_type() == MoveEvent::TYPE) {
 *     const MoveEvent& data = get_data<MoveEvent>(ev);
 *     std::cout << data.point.x << '\n';
 *   }
 * }
 * ```
 */
template<class EventDataType>
const EventDataType& get_data(const Event& event) {
    if (event.get_type() != EventDataType::TYPE) {
        throw std::bad_cast();
    }
    return *static_cast<const EventDataType*>(&event.get_data());
}
