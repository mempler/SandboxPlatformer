#pragma once

#include <string_view>

#include "steam/steamnetworkingtypes.h"

#include "Packet.hh"

#include <steam/isteamnetworkingsockets.h>

#include <signals.hpp>

enum class ConnectionState
{
    Disconnected,
    Connecting,
    Connected
};

class BaseClient
{
  public:
    typedef std::shared_ptr<BaseClient> BaseClientPtr;

    BaseClient()
    {
    }
    BaseClient( ISteamNetworkingSockets *pInstance, HSteamNetConnection hConn ) :
        m_pInstance( pInstance ), m_hConn( hConn )
    {
    }

    void Send( IBasePacket *pPacket )
    {
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

    bool IsConnected()
    {
        SteamNetworkingQuickConnectionStatus *pStatus = nullptr;
        if ( !m_pInstance->GetQuickConnectionStatus( m_hConn, pStatus ) ) return false;
        if ( pStatus == nullptr ) return false;

        return pStatus->m_eState == k_ESteamNetworkingConnectionState_Connected;
    }

    bool IsConnecting()
    {
        SteamNetworkingQuickConnectionStatus *pStatus = nullptr;
        if ( !m_pInstance->GetQuickConnectionStatus( m_hConn, pStatus ) ) return false;

        return pStatus->m_eState == k_ESteamNetworkingConnectionState_Connecting;
    }

    bool IsDisconnected()
    {
        return !( IsConnecting() && IsConnected() );
    }

    signals::signal<void( BaseClientPtr, ConnectionState, const char * )> OnStateChange;

  private:
    HSteamNetConnection m_hConn = 0;
    ISteamNetworkingSockets *m_pInstance = nullptr;
};

using BaseClientPtr = BaseClient::BaseClientPtr;
