#pragma once
#include "messaging_driver.hpp"

struct GameServerOptions {};

class GameServer {
    IMessagingDriver &_driver;

  public:
    GameServer(IMessagingDriver &driver, const GameServerOptions &opts);
    ~GameServer();

    void run();
};
