#include "BaseNetwork.hh"

#include <steam/isteamnetworkingsockets.h>
#include <steam/steamnetworkingsockets.h>
#include <steam/steamnetworkingtypes.h>

static BaseNetwork *g_pActiveNetwork;

BaseNetwork::BaseNetwork()
{
    SteamDatagramErrMsg errMsg;
    if ( !GameNetworkingSockets_Init( nullptr, errMsg ) )
        Console::Fatal( "GameNetworkingSockets_Init failed: {}", errMsg );

    m_pInstance = SteamNetworkingSockets();
}

BaseClientPtr BaseNetwork::ConnectTo( SteamNetworkingIPAddr &address )
{
    SteamNetworkingConfigValue_t opt;
    opt.SetPtr( k_ESteamNetworkingConfig_Callback_ConnectionStatusChanged,
                (void *) &BaseNetwork::OnStatusChanged );

    HSteamNetConnection hConn = m_pInstance->ConnectByIPAddress( address, 1, &opt );

    return AddConnection( hConn );
}

void BaseNetwork::Tick()
{
    g_pActiveNetwork = this;
    m_pInstance->RunCallbacks();
}

BaseClientPtr BaseNetwork::AddConnection( HSteamNetConnection hConn )
{
    m_umConnectedClients.emplace( hConn,
                                  std::make_shared<BaseClient>( m_pInstance, hConn ) );

    return m_umConnectedClients.at( hConn );
}

BaseClientPtr BaseNetwork::GetConnection( HSteamNetConnection hConn )
{
    auto it = m_umConnectedClients.find( hConn );
    if ( it != m_umConnectedClients.end() )
    {
        return it->second;
    }

    return nullptr;
}

void BaseNetwork::DestroyConnection( HSteamNetConnection hConn )
{
    BaseClientPtr client = GetConnection( hConn );
    if ( client == nullptr || !client->IsValid() ) return;

    // We cannot destroy connections
    // This technically creates a memory leak
    // but we dont fucking care :)
    m_umConnectedClients.erase( hConn );
}

/* static */
void BaseNetwork::OnStatusChanged( SteamNetConnectionStatusChangedCallback_t *pInfo )
{
    switch ( pInfo->m_info.m_eState )
    {
    case k_ESteamNetworkingConnectionState_ClosedByPeer:
    case k_ESteamNetworkingConnectionState_ProblemDetectedLocally:
    {
        auto conn = g_pActiveNetwork->GetConnection( pInfo->m_hConn );
        conn->OnStateChange( conn, ConnectionState::Disconnected,
                             (const char *) pInfo->m_info.m_szEndDebug );

        g_pActiveNetwork->OnStateChange( conn, ConnectionState::Disconnected,
                                         (const char *) pInfo->m_info.m_szEndDebug );

        conn->Disconnect( pInfo->m_info.m_szEndDebug );
        g_pActiveNetwork->DestroyConnection( pInfo->m_hConn );
        break;
    }

    case k_ESteamNetworkingConnectionState_Connecting:
    {
        Console::Info( "We're trying to establish an connection! ({})",
                       pInfo->m_info.m_szEndDebug );

        auto conn = g_pActiveNetwork->GetConnection( pInfo->m_hConn );
        conn->OnStateChange( conn, ConnectionState::Connecting,
                             (const char *) pInfo->m_info.m_szEndDebug );

        g_pActiveNetwork->OnStateChange( conn, ConnectionState::Connecting,
                                         (const char *) pInfo->m_info.m_szEndDebug );

        break;
    }

    case k_ESteamNetworkingConnectionState_Connected:
    {
        Console::Info( "We established an connection! ({})", pInfo->m_info.m_szEndDebug );

        auto conn = g_pActiveNetwork->GetConnection( pInfo->m_hConn );
        conn->OnStateChange( conn, ConnectionState::Connected,
                             (const char *) pInfo->m_info.m_szEndDebug );

        g_pActiveNetwork->OnStateChange( conn, ConnectionState::Connected,
                                         (const char *) pInfo->m_info.m_szEndDebug );
        break;
    }

    default: break; /* Unhandled */
    }
}