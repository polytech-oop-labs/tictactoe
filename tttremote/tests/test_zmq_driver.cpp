#include "zmq_drivers.hpp"
#include <iostream>
#include "debug_listener.hpp"


int main()
{
    zmq::context_t ctx;
    ZMQMessagingDriver drv(ctx, "tcp://*:5555");
    std::cout << "Waiting for connections...\n";
    UpdateListener lst;
    ClientId last_client;
    ConcreteMessage msg(std::vector<std::string>{"Hello!", "client!"});
    while (!lst.checkMessage(last_client))
    {
        drv.handleUpdate(lst, 100);
    }
    drv.sendMessage(last_client, msg, MessageSendingOptions{});
}
