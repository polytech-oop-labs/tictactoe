#include "zmq_drivers.hpp"
#include "messaging_driver.hpp"
#include "zmq.hpp"

class ZMQMessage : public IMessage {
    zmq::message_t *_last_message;
    zmq::socket_t &_socket;
    bool _has_next;

  public:
    ZMQMessage(zmq::message_t &last_message, zmq::socket_t &socket);
    ZMQMessage(zmq::socket_t &socket);
    ~ZMQMessage();
    bool hasNextFrame() const override;
    MessagePart getNextFrame() override;
};

ZMQMessage::ZMQMessage(zmq::message_t &last_message, zmq::socket_t &socket)
    : _last_message(0), _socket(socket), _has_next(last_message.more())
{}

ZMQMessage::ZMQMessage(zmq::socket_t &socket)
    : _last_message(0), _socket(socket), _has_next(true)
{}

bool ZMQMessage::hasNextFrame() const
{
    return _has_next;
}

ZMQMessage::MessagePart ZMQMessage::getNextFrame()
{
    delete _last_message;
    _last_message = new zmq::message_t;
    (void)_socket.recv(*_last_message);
    _has_next = _last_message->more();
    return {
        static_cast<const char *>(_last_message->data()),
        _last_message->size(),
    };
}

ZMQMessage::~ZMQMessage()
{
    delete _last_message;
}

bool ZMQMessagingDriver::MessageTimeout::operator<(
    const MessageTimeout &other) const
{
    return timeout_timepoint < other.timeout_timepoint;
}

ZMQMessagingDriver::ZMQMessagingDriver(zmq::context_t &ctx,
                                       const char *socker_url)
    : _last_id(0), _socket(ctx, zmq::socket_type::router)
{
    _socket.bind(socker_url);
}

MessageId ZMQMessagingDriver::sendMessage(const ClientId &client_id,
                                          IMessage &msg,
                                          const MessageSendingOptions &opts)
{
    MessageId id = ++_last_id;
    if (opts.timeout_ms > 0)
    {
        auto now = std::chrono::steady_clock::now();
        MessageTimeout timeout_record{
            now + std::chrono::milliseconds(opts.timeout_ms), id, client_id};
        _timeouts_queue.push(timeout_record);
    }
    _socket.send(zmq::message_t(client_id.begin(), client_id.end()),
                 zmq::send_flags::sndmore);
    bool have_next_frame = msg.hasNextFrame();
    _socket.send(zmq::message_t(), have_next_frame ? zmq::send_flags::sndmore
                                                   : zmq::send_flags::none);
    while (have_next_frame)
    {
        auto frame = msg.getNextFrame();
        zmq::message_t buf(frame.data, frame.size);
        have_next_frame = msg.hasNextFrame();
        _socket.send(buf, have_next_frame ? zmq::send_flags::sndmore
                                          : zmq::send_flags::none);
    }
    return id;
}

void ZMQMessagingDriver::handleUpdate(IMessageUpdateListener &update_listener,
                                      size_t max_timeout_ms)
{
    bool expecting_msg_timeout = false;
    if (!_timeouts_queue.empty())
    {
        auto now = std::chrono::steady_clock::now();
        auto &next_timeout = _timeouts_queue.top();
        int next_timeout_ms =
            std::chrono::duration_cast<std::chrono::milliseconds>(
                next_timeout.timeout_timepoint - now)
                .count();
        if (next_timeout_ms < 0)
        {
            max_timeout_ms = 0;
            expecting_msg_timeout = true;

        } else if (next_timeout_ms <= max_timeout_ms)
        {
            max_timeout_ms = next_timeout_ms;
            expecting_msg_timeout = true;
        }
    }
    zmq::pollitem_t pollitem{_socket, 0, ZMQ_POLLIN, 0};
    zmq::poll(&pollitem, 1, std::chrono::milliseconds(max_timeout_ms));
    if (pollitem.revents & ZMQ_POLLIN)
    {
        zmq::message_t client_id_frame, empty_frame;
        auto res = _socket.recv(client_id_frame);
        if (!client_id_frame.more())
        {
            throw "Expected another frame";
        }
        ClientId client_id{};
        auto size = std::min(client_id_frame.size(), client_id.size());
        for (int i = 0; i < size; ++i)
        {
            client_id[i] = static_cast<char *>(client_id_frame.data())[i];
        }
        (void)_socket.recv(empty_frame);
        ZMQMessage zmq_msg(empty_frame, _socket);
        update_listener.handleNewMessage(client_id, zmq_msg);
        if (expecting_msg_timeout)
        {
            if (client_id == _timeouts_queue.top().client)
            {
                _timeouts_queue.pop();
            }
        }
    } else if (expecting_msg_timeout)
    {
        update_listener.handleMessageTimeout(_timeouts_queue.top().message);
        _timeouts_queue.pop();
    }
}

ZMQRequestSender::ZMQRequestSender(zmq::context_t &ctx, const char *socker_url)
    : _socket(ctx, zmq::socket_type::req)
{
    _socket.connect(socker_url);
}

void ZMQRequestSender::sendRequest(IMessage &msg,
                                   const MessageSendingOptions &opts,
                                   IMessageUpdateListener &update_listener)
{
    bool has_next = msg.hasNextFrame();
    if (!has_next)
    {
        throw "Empty messages are not allowed";
    }
    while (has_next)
    {
        auto frame = msg.getNextFrame();
        zmq::message_t buf(frame.data, frame.size);
        has_next = msg.hasNextFrame();
        _socket.send(buf, has_next ? zmq::send_flags::sndmore
                                   : zmq::send_flags::none);
    }
    zmq::pollitem_t pollitem{_socket, 0, ZMQ_POLLIN, 0};
    long timeout = opts.timeout_ms;
    if (timeout == 0)
    {
        timeout = -1;
    }
    zmq::poll(&pollitem, 1, std::chrono::milliseconds(timeout));
    if (pollitem.revents & ZMQ_POLLIN)
    {
        ZMQMessage recv_msg(_socket);
        update_listener.handleNewMessage(ClientId{}, recv_msg);
    } else
    {
        update_listener.handleMessageTimeout(MessageId{});
    }
}
