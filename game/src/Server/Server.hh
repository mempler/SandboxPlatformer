#pragma once

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

    ISteamNetworkingSockets *m_pSteamSockets;

    HSteamListenSocket m_hSocket;
    HSteamNetPollGroup m_hPollGroup;

    // Valve, what the fuck????
    static void OnStatusChanged( SteamNetConnectionStatusChangedCallback_t *pStatus );
};

Server *GetServer();
