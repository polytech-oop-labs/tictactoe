#include "messages.hpp"
#include <array>
#include <bit>

std::string convertMessageType(MessageType type)
{
    int32_t t = static_cast<int32_t>(type);
    char *ptr = reinterpret_cast<char *>(&t);
    return std::string(ptr, ptr + sizeof(t));
}

MessageType MessageDecoder::_tryParseType(IMessage &msg)
{
    if (!msg.hasNextFrame())
        return MessageType::CANNOT_DECODE;
    IMessage::MessagePart first_part = msg.getNextFrame();
    if (first_part.size != sizeof(MessageType))
        return MessageType::CANNOT_DECODE;
    int32_t underlying_type =
        *reinterpret_cast<const int32_t *>(first_part.data);
    if (underlying_type <= 0 ||
        underlying_type >= static_cast<int32_t>(MessageType::_MAX_VALID_TYPE))
        return MessageType::CANNOT_DECODE;
    return static_cast<MessageType>(underlying_type);
}

MessageDecoder::MessageDecoder(IMessage &msg) : _msg(msg)
{
    _type = _tryParseType(msg);
}

MessageType MessageDecoder::getType() const
{
    return _type;
}

std::unique_ptr<IMessage> MessageCreator::createMessage(
    const PlayerEnterMessage &msg)
{
    char mark;
    switch (msg.player_mark)
    {
    case Mark::Cross:
        mark = 1;
        break;
    case Mark::Zero:
        mark = 2;
        break;
    case Mark::None:
        mark = 0;
        break;
    }
    std::vector<std::string> parts{convertMessageType(MessageType::ENTP),
                                   msg.name, std::string(&mark, &mark + 1)};
    return std::make_unique<ConcreteMessage>(parts);
}

std::unique_ptr<PlayerEnterMessage> MessageDecoder::getPlayerEnterMessage()
{
    if (_type != MessageType::ENTP)
        return nullptr;
    if (!_msg.hasNextFrame())
        return nullptr;
    IMessage::MessagePart name_part = _msg.getNextFrame();
    if (name_part.size == 0)
        /// players with empty names are not allowed
        return nullptr;
    if (!_msg.hasNextFrame())
        return nullptr;
    IMessage::MessagePart mark_part = _msg.getNextFrame();
    if (mark_part.size != 1)
        return nullptr;
    Mark mark = Mark::None;
    switch (mark_part.data[0])
    {
    case 0:
        break;
    case 1:
        mark = Mark::Cross;
        break;
    case 2:
        mark = Mark::Zero;
        break;
    default:
        return nullptr;
    }
    std::string name(name_part.data, name_part.size);
    return std::make_unique<PlayerEnterMessage>(name, mark);
}
