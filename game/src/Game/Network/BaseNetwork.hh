#pragma once

#include <unordered_map>

#include "steam/steamnetworkingtypes.h"

#include "Core/Utils/Logger.hh"

#include "Packet.hh"

#include "BaseClient.hh"
#include "BaseServer.hh"

#include <steam/isteamnetworkingsockets.h>

#include <signals.hpp>

class BaseNetwork
{
  public:
    BaseNetwork();

    // Connect to a remote server using it's IP
    // The resulting BaseClient can then be used to send
    // messages over the network
    BaseClientPtr ConnectTo( SteamNetworkingIPAddr &address );

    BaseServer CreateServer( SteamNetworkingIPAddr &address );

    // This has to be called to send all remaining packets
    // towards the server (every tick preferable)
    void Tick();

    signals::signal<void( BaseClientPtr, ConnectionState, const char * )> OnStateChange;
    signals::signal<void( BaseClientPtr, Kokoro::Memory::Buffer & )> OnPacket;

  private:
    BaseClientPtr AddConnection( HSteamNetConnection hConn );
    void DestroyConnection( HSteamNetConnection hConn );
    BaseClientPtr GetConnection( HSteamNetConnection hConn );

    std::unordered_map<HSteamNetConnection, BaseClientPtr> m_umConnectedClients {};

    ISteamNetworkingSockets *m_pInstance = nullptr;
    HSteamNetPollGroup m_hPollGroup = k_HSteamNetPollGroup_Invalid;

    static void OnStatusChanged( SteamNetConnectionStatusChangedCallback_t *pInfo );
};
