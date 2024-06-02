#include "game_server.hpp"
#include "player.h"

class WaitingUpdateHandler : public IMessageUpdateListener {
    // ...

  public:
};

class RemotePlayer : public Player {
    class PlayerUpdateHandler : public IMessageUpdateListener {};
    ClientId _client_id;
    IMessagingDriver &_driver;


  public:
    RemotePlayer(const ClientId &client, IMessagingDriver &driver);
};
