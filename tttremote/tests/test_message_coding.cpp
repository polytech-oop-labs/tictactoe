#include "messages.hpp"

/// encode and decode valid message and make sure they are identical
/// test should return 1 if test fails
int test_player_enter_message()
{
    std::string name = "vasya";
    for (Mark mark : {Mark::Zero, Mark::Cross, Mark::None})
    {
        PlayerEnterMessage in_msg{name, mark};
        MessageCreator creator;
        auto encoded_msg = creator.createMessage(in_msg);
        MessageDecoder decoder(*encoded_msg);
        if (decoder.getType() != MessageType::ENTP)
            return 1;
        auto out_msg = decoder.getPlayerEnterMessage();
        if (!out_msg)
            return 1;
        if (out_msg->name != name)
            return 1;
        if (out_msg->player_mark != mark)
            return 1;
    }
    return 0;
}

// main of unit test returns 1 if test failed or 0 if everything is fine
int main()
{
    if (test_player_enter_message())
        return 1;
    return 0;
}
