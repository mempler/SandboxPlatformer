#pragma once

#include <string_view>

#include "Packet.hh"

#include <Tracy.hpp>
#define ENET_IMPLEMENTATION
#include <enet.h>
#include <signals.hpp>

enum class ConnectionState
{
    Disconnected,
    Connecting,
    Connected
};

constexpr const char *ConnectionState_ToString( ConnectionState eState )
{
    switch ( eState )
    {
    case ConnectionState::Disconnected: return "Disconnected";
    case ConnectionState::Connected: return "Connected";
    case ConnectionState::Connecting: return "Connecting";
    }

    return "Unknown";
}

class NetClient
{
  public:
    typedef std::shared_ptr<NetClient> NetClientPtr;

    NetClient()
    {
    }
    NetClient( ENetHost *pInstance, ENetPeer *hConn ) :
        m_pInstance( pInstance ), m_hConn( hConn )
    {
    }

    template <class P>
    size_t Send( P &packet )
    {
        ZoneScoped;

        return packet.SendTo( m_pInstance, m_hConn );
    }

    void Disconnect( std::string_view const &svReason )
    {
        ZoneScoped;

        enet_host_destroy( m_pInstance );
        enet_peer_reset( m_hConn );
    }

    bool IsValid()
    {
        ZoneScoped;

        return m_hConn->state == ENET_PEER_STATE_CONNECTED;
    }

    ConnectionState GetState()
    {
        ZoneScoped;

        switch ( m_hConn->state )
        {
        case ENET_PEER_STATE_CONNECTED: return ConnectionState::Connected; break;
        case ENET_PEER_STATE_CONNECTING: return ConnectionState::Connecting; break;
        case ENET_PEER_STATE_DISCONNECTED: return ConnectionState::Disconnected; break;
        default: return ConnectionState::Disconnected;
        }
    }

    ENetPeer *Handle()
    {
        ZoneScoped;

        return m_hConn;
    }

    signals::signal<void( NetClientPtr, ConnectionState, const char * )> OnStateChange;
    signals::signal<void( NetClientPtr, PacketHeader, Kokoro::Memory::Buffer )> OnPacket;

  private:
    ENetPeer *m_hConn = 0;
    ENetHost *m_pInstance = 0;
};

using NetClientPtr = NetClient::NetClientPtr;