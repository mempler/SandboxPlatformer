#pragma once

#include <unordered_map>

#include "Core/Utils/Logger.hh"

#include "Game/Network/NetClient.hh"
#include "Game/Network/Packet.hh"

#include <enet/enet.h>
#include <signals.hpp>

class Network
{
  public:
    Network();

    // Connect to a remote server using it's IP
    // The resulting BaseClient can then be used to send
    // messages over the network
    NetClientPtr ConnectTo( ENetAddress &address );

    // This has to be called to send all remaining packets
    // towards the server (every tick preferable)
    void Tick();

    signals::signal<void( NetClientPtr, ConnectionState, const char * )> OnStateChange;
    signals::signal<void( NetClientPtr, PacketHeader, Kokoro::Memory::Buffer )> OnPacket;

  private:
    ENetHost *m_pInstance = nullptr;

    static void OnStatusChanged();
};
