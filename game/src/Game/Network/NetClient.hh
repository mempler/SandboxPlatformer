#pragma once

#include <string_view>

#include "Packet.hh"

#include <steam/isteamnetworkingsockets.h>
#include <steam/steamnetworkingtypes.h>

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
    NetClient( ISteamNetworkingSockets *pInstance, HSteamNetConnection hConn ) :
        m_pInstance( pInstance ), m_hConn( hConn )
    {
    }

    template <class P>
    size_t Send( P &packet )
    {
        return packet.SendTo( m_pInstance, m_hConn );
    }

    void Disconnect( std::string_view const &svReason )
    {
        m_pInstance->CloseConnection( m_hConn, k_ESteamNetConnectionEnd_App_Generic,
                                      svReason.data(), true );

        m_hConn = k_HSteamNetConnection_Invalid;
    }

    bool IsValid()
    {
        return m_hConn != k_HSteamNetConnection_Invalid;
    }

    ConnectionState GetState()
    {
        SteamNetworkingQuickConnectionStatus *pStatus = nullptr;
        if ( !m_pInstance->GetQuickConnectionStatus( m_hConn, pStatus ) )
        {
            return ConnectionState::Disconnected;
        }
        if ( pStatus == nullptr )
        {
            return ConnectionState::Disconnected;
        }

        switch ( pStatus->m_eState )
        {
        case k_ESteamNetworkingConnectionState_Connected:
            return ConnectionState::Connected;
            break;
        case k_ESteamNetworkingConnectionState_Connecting:
            return ConnectionState::Connecting;
            break;
        case k_ESteamNetworkingConnectionState_ClosedByPeer:
        case k_ESteamNetworkingConnectionState_ProblemDetectedLocally:
            return ConnectionState::Disconnected;
            break;
        default: return ConnectionState::Disconnected;
        }
    }

    HSteamNetConnection Handle()
    {
        return m_hConn;
    }

    signals::signal<void( NetClientPtr, ConnectionState, const char * )> OnStateChange;
    signals::signal<void( NetClientPtr, PacketHeader, Kokoro::Memory::Buffer )> OnPacket;

  private:
    HSteamNetConnection m_hConn = 0;
    ISteamNetworkingSockets *m_pInstance = nullptr;
};

using NetClientPtr = NetClient::NetClientPtr;
