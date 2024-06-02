#include "debug_listener.hpp"
#include "zmq_drivers.hpp"
#include <iostream>

int main()
{
    zmq::context_t ctx;
    ZMQRequestSender sender(ctx, "tcp://0.0.0.0:5555");
    UpdateListener lst;
    ConcreteMessage msg(std::vector<std::string>{"Hello!", "from", "client!"});
    sender.sendRequest(msg, MessageSendingOptions{}, lst);
    return 0;
}
