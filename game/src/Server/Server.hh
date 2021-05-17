#pragma once

#include "Game/Network/BaseNetwork.hh"
#include "Game/World/World.hh"

#include <steam/steamnetworkingsockets.h>

class Server
{
  public:
    void Init();
    void Tick( float fDeltaTime );
    void Run();

  public:
    ItemInfoManager &GetItemInfoMan()
    {
        return m_ItemInfoManager;
    }

  private:
    ItemInfoManager m_ItemInfoManager;
    World m_World;

    BaseServer m_Server;
    BaseNetwork m_Network;

    void OnStateChange( BaseClientPtr pClient, ConnectionState eState,
                        const char *szMessage );
};

Server *GetServer();
