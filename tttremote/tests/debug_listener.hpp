#pragma once
#include "messaging_driver.hpp"
#include <iostream>

class UpdateListener : public IMessageUpdateListener {
    bool _has_message;
    ClientId _last_client;

  public:
    UpdateListener() : _has_message(false)
    {}
    void handleNewMessage(const ClientId &client_id, IMessage &msg)
    {
        std::cout << "Client:";
        for (auto c : client_id)
        {
            std::cout << ' ' << static_cast<int>(c);
        }
        std::cout << '\n';
        _has_message = true;
        _last_client = client_id;
        int n_part = 0;
        while (msg.hasNextFrame())
        {
            auto frame = msg.getNextFrame();
            std::cout << "Part " << ++n_part << ": ";
            std::cout.write(frame.data, frame.size);
            std::cout << '\n';
        }
    }
    void handleMessageTimeout(const MessageId msg_id)
    {
        std::cout << "Timeout for message: " << msg_id << '\n';
    }
    bool checkMessage(ClientId &last_client)
    {
        bool result = _has_message;
        if (result)
        {
            last_client = _last_client;
        }
        _has_message = false;
        return result;
    }
};
