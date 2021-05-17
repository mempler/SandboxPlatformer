#pragma once

#include "Game/Network/Network.hh"
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

    NetListener m_Listener;
    Network m_Network;

    void OnStateChange( NetClientPtr pClient, ConnectionState eState,
                        const char *szMessage );
};

Server *GetServer();
