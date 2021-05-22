#pragma once

#include <unordered_map>
#include <vector>

#include "Core/Utils/Logger.hh"

#include "Network/NetClient.hh"
#include "Network/Packet.hh"

#include <enet/enet.h>

#include <signals.hpp>
#include <stdint.h>

class Network
{
  public:
    Network();

    // Initialize client connection
    void InitClient();

    // Initialize server connection
    void InitServer( ENetAddress &address, uint32_t uMaxConnection );

    // Connect to a remote server using it's IP
    // The resulting BaseClient can then be used to send
    // messages over the network
    NetClientPtr ConnectTo( ENetAddress &address );

    // Add new peer to our network
    NetClientPtr AddPeer( ENetPeer *peer );

    // This has to be called to send all remaining packets
    // towards the server (every tick preferable)
    void Tick();

    signals::signal<void( NetClientPtr, ConnectionState )> OnStateChange;
    signals::signal<void( NetClientPtr, PacketHeader, Kokoro::Memory::Buffer )> OnPacket;

  private:
    bool m_bIsClient = false;

    ENetHost *m_pInstance = nullptr;

    std::vector<NetClientPtr> m_vClients;
};
