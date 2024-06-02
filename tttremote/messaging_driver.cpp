#include "messaging_driver.hpp"

ConcreteMessage::ConcreteMessage(const std::vector<std::string> &msg)
    : _msg(msg), _current_frame(0)
{}


bool ConcreteMessage::hasNextFrame() const {
    return _current_frame < _msg.size();
}
ConcreteMessage::MessagePart ConcreteMessage::getNextFrame() {
    MessagePart res;
    res.data = _msg.at(_current_frame).c_str();
    res.size = _msg.at(_current_frame).size();
    ++_current_frame;
    return res;
}
