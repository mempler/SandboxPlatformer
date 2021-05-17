#pragma once

#include <unordered_map>

#include "steam/steamnetworkingtypes.h"

#include "Core/Utils/Logger.hh"

#include "Game/Network/NetClient.hh"
#include "Game/Network/NetListener.hh"
#include "Game/Network/Packet.hh"

#include <steam/isteamnetworkingsockets.h>

#include <signals.hpp>

class Network
{
  public:
    Network();

    // Connect to a remote server using it's IP
    // The resulting BaseClient can then be used to send
    // messages over the network
    NetClientPtr ConnectTo( SteamNetworkingIPAddr &address );

    NetListener CreateListener( SteamNetworkingIPAddr &address );

    // This has to be called to send all remaining packets
    // towards the server (every tick preferable)
    void Tick();

    signals::signal<void( NetClientPtr, ConnectionState, const char * )> OnStateChange;
    signals::signal<void( NetClientPtr, PacketHeader, Kokoro::Memory::Buffer )> OnPacket;

  private:
    NetClientPtr AddConnection( HSteamNetConnection hConn );
    void DestroyConnection( HSteamNetConnection hConn );
    NetClientPtr GetConnection( HSteamNetConnection hConn );

    std::unordered_map<HSteamNetConnection, NetClientPtr> m_umConnectedClients {};

    ISteamNetworkingSockets *m_pInstance = nullptr;

    HSteamNetPollGroup m_hPollGroup = k_HSteamNetPollGroup_Invalid;

    static void OnStatusChanged( SteamNetConnectionStatusChangedCallback_t *pInfo );
};
