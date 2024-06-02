/**
 * This file contains messaging abstractions to be used for remote game
 */
#pragma once
#include <array>
#include <cstddef>
#include <string>
#include <vector>

using MessageId = int;
using ClientId = std::array<char, 5>;

/**
 * This is interface for both ingoing and outgoing multipart message
 */
class IMessage {
  public:
    struct MessagePart {
        const char *data;
        size_t size;
    };

    virtual ~IMessage()
    {}

    virtual bool hasNextFrame() const = 0;
    virtual MessagePart getNextFrame() = 0;
};

/**
 * This is inferface for handling different updates that can happen with message
 * driver.
 *
 * There are currently next types of updates:
 * - new incoming message
 * - timeout for response from client
 */
class IMessageUpdateListener {
  public:
    virtual ~IMessageUpdateListener()
    {}

    virtual void handleNewMessage(const ClientId &client_id,
                                  IMessage &msg)
    {}
    virtual void handleMessageTimeout(const MessageId msg_id)
    {}
};

struct MessageSendingOptions {
    size_t timeout_ms = 0;
};

/**
 * This is interface for general messaging driver.
 */
class IMessagingDriver {
  public:
    virtual ~IMessagingDriver()
    {}
    /// Send message to given client with given options
    virtual MessageId sendMessage(const ClientId &client_id, IMessage &msg,
                                  const MessageSendingOptions &opts) = 0;
    /// Handle exactly one next update
    virtual void handleUpdate(IMessageUpdateListener &update_listener,
                              size_t max_timeout_ms) = 0;
};

/**
 * This is interface for a request sender that is connected to one peer.
 */
class IRequestSender {
  public:
    virtual ~IRequestSender()
    {}
    virtual void sendRequest(IMessage &msg, const MessageSendingOptions &opts,
                             IMessageUpdateListener &update_listener) = 0;
};

class ConcreteMessage : public IMessage {
    std::vector<std::string> _msg;
    size_t _current_frame;

  public:
    ConcreteMessage(const std::vector<std::string> &msg);
    bool hasNextFrame() const override;
    MessagePart getNextFrame() override;
};
