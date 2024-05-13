#pragma once
#include "game_state.h"
#include "messaging_driver.hpp"
#include <cstdint>
#include <memory>

enum class MessageType : int32_t {
    ENTP = 1,
    ENTO = 2,
    STAT = 3,
    EVNT = 4,
    PLAY = 5,
    OK = 6,
    ACPT = 7,
    RJCT = 8,
    MKTK = 9,
    SNDS = 10,
    MOVE = 11,
    FAIL = 12,
    _MAX_VALID_TYPE,
    CANNOT_DECODE = 999,
};

using EventNumber = uint32_t;

struct PlayerEnterMessage {
    std::string name;
    Mark player_mark;
};

struct ObserverEnterMessage {};

struct StateMessage {
    EventNumber last_event;
    std::string cross_player;
    std::string zero_player;
    uint32_t cross_wins;
    uint32_t zero_wins;
    GameSettings settings;
    GameState state;
};

struct EventMessage {
    EventNumber event_number;
    Event event;
};

struct PlayMessage {
    EventNumber last_event;
};

struct OkMessage {};

struct AcceptMessage {
    Mark player_mark;
    EventNumber last_event;
    GameSettings settings;
};

struct RejectMessage {
    std::string reason;
};

struct MarkTakenMessage {
    std::string reason;
};

struct SendStateMessage {};

struct MoveMessage {
    Point move;
};

struct FailMessage {
    std::string reason;
};

class MessageCreator {
  public:
    std::unique_ptr<IMessage> createMessage(const PlayerEnterMessage &msg);
    std::unique_ptr<IMessage> createMessage(const ObserverEnterMessage &msg); // TODO
    std::unique_ptr<IMessage> createMessage(const StateMessage &msg); // TODO
    std::unique_ptr<IMessage> createMessage(const EventMessage &msg); // TODO
    std::unique_ptr<IMessage> createMessage(const PlayMessage &msg); // TODO
    std::unique_ptr<IMessage> createMessage(const OkMessage &msg); // TODO
    std::unique_ptr<IMessage> createMessage(const AcceptMessage &msg); // TODO
    std::unique_ptr<IMessage> createMessage(const RejectMessage &msg); // TODO
    std::unique_ptr<IMessage> createMessage(const MarkTakenMessage &msg); // TODO
    std::unique_ptr<IMessage> createMessage(const SendStateMessage &msg); // TODO
    std::unique_ptr<IMessage> createMessage(const MoveMessage &msg); // TODO
    std::unique_ptr<IMessage> createMessage(const FailMessage &msg); // TODO
};

class MessageDecoder {
    IMessage& _msg;
    MessageType _type;

    MessageType _tryParseType(IMessage& msg);
public:
    MessageDecoder(IMessage& msg);
    MessageType getType() const;
    std::unique_ptr<PlayerEnterMessage> getPlayerEnterMessage();
    std::unique_ptr<ObserverEnterMessage> getObserverEnterMessage(); // TODO
    std::unique_ptr<StateMessage> getStateMessage(); // TODO
    std::unique_ptr<EventMessage> getEventMessage(); // TODO
    std::unique_ptr<PlayMessage> getPlayMessage(); // TODO
    std::unique_ptr<OkMessage> getOkMessage(); // TODO
    std::unique_ptr<AcceptMessage> getAcceptMessage(); // TODO
    std::unique_ptr<RejectMessage> getRejectMessage(); // TODO
    std::unique_ptr<MarkTakenMessage> getMarkTakenMessage(); // TODO
    std::unique_ptr<SendStateMessage> getSendStateMessage(); // TODO
    std::unique_ptr<MoveMessage> getMoveMessage(); // TODO
    std::unique_ptr<FailMessage> getFailMessage(); // TODO
};
