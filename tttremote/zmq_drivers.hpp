#pragma once
#include "messaging_driver.hpp"
#include <chrono>
#include <queue>
#include <zmq.hpp>

class ZMQMessagingDriver : public IMessagingDriver {
    zmq::socket_t _socket;

    struct MessageTimeout {
        std::chrono::steady_clock::time_point timeout_timepoint;
        MessageId message;
        ClientId client;

        bool operator<(const MessageTimeout &other) const;
    };

    std::priority_queue<MessageTimeout> _timeouts_queue;
    MessageId _last_id;

  public:
    ZMQMessagingDriver(zmq::context_t &ctx, const char *socker_url);
    MessageId sendMessage(const ClientId &client_id, IMessage &msg,
                          const MessageSendingOptions &opts) override;
    void handleUpdate(IMessageUpdateListener &update_listener,
                      size_t max_timeout_ms) override;
};

class ZMQRequestSender : public IRequestSender {
    zmq::socket_t _socket;

  public:
    ZMQRequestSender(zmq::context_t &ctx, const char *socker_url);
    void sendRequest(IMessage &msg, const MessageSendingOptions &opts,
                     IMessageUpdateListener &update_listener) override;
};
