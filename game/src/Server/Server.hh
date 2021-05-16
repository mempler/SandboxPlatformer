#pragma once

#include <steam/steamnetworkingsockets.h>

class Server
{
  public:
    void Init();
    void Tick( float fDeltaTime );
    void Run();

  private:
    ISteamNetworkingSockets *m_pSteamSockets;

    HSteamListenSocket m_hSocket;
    HSteamNetPollGroup m_hPollGroup;

    // Valve, what the fuck????
    static void OnStatusChanged( SteamNetConnectionStatusChangedCallback_t *pStatus );
};

Server *GetServer();
