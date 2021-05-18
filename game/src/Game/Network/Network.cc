#include "Network.hh"

#include <steam/isteamnetworkingmessages.h>
#include <steam/isteamnetworkingsockets.h>
#include <steam/isteamnetworkingutils.h>
#include <steam/steamnetworkingsockets.h>
#include <steam/steamnetworkingsockets_flat.h>
#include <steam/steamnetworkingtypes.h>

#include <Tracy.hpp>

#include <array>

static Network *g_pActiveNetwork;

Network::Network()
{
    ZoneScoped;

    SteamDatagramErrMsg errMsg;
    if ( !GameNetworkingSockets_Init( nullptr, errMsg ) )
        Console::Fatal( "GameNetworkingSockets_Init failed: {}", errMsg );

    m_pInstance = SteamNetworkingSockets();

    m_hPollGroup = m_pInstance->CreatePollGroup();
}

NetClientPtr Network::ConnectTo( SteamNetworkingIPAddr &address )
{
    ZoneScoped;

    SteamNetworkingConfigValue_t opt;
    opt.SetPtr( k_ESteamNetworkingConfig_Callback_ConnectionStatusChanged,
                (void *) &Network::OnStatusChanged );

    HSteamNetConnection hConn = m_pInstance->ConnectByIPAddress( address, 1, &opt );

    return AddConnection( hConn );
}

NetListener Network::CreateListener( SteamNetworkingIPAddr &address )
{
    ZoneScoped;

    SteamNetworkingConfigValue_t opt;
    opt.SetPtr( k_ESteamNetworkingConfig_Callback_ConnectionStatusChanged,
                (void *) &Network::OnStatusChanged );

    HSteamListenSocket hSocket = m_pInstance->CreateListenSocketIP( address, 1, &opt );
    if ( hSocket == k_HSteamListenSocket_Invalid )
    {
        Console::Fatal( "Failed to listen on port {}", 27015 );
    }

    m_pInstance->SetConnectionPollGroup( hSocket, m_hPollGroup );

    return NetListener( m_pInstance, hSocket );
}

void Network::Tick()
{
    ZoneScoped;

    // Accept messages
    ISteamNetworkingMessage *pIncomingMsg = nullptr;
    m_pInstance->ReceiveMessagesOnPollGroup( m_hPollGroup, &pIncomingMsg, 1 );

    if ( pIncomingMsg != nullptr )
    {
        Kokoro::Memory::Buffer buffer( (uint8_t *) pIncomingMsg->GetData(),
                                       pIncomingMsg->GetSize() );

        PacketHeader header;
        if ( !header.Unpack( buffer ) )
        {
            Console::Error( "Invalid message! skipping..." );
        }
        else
        {
            NetClientPtr client = GetConnection( pIncomingMsg->GetConnection() );

            pIncomingMsg->Release();  // Release resources we dont need it anymore

            OnPacket( client, header,
                      buffer );  // Let our signal subscriber handle all this.
            client->OnPacket( client, header, buffer );
        }
    }

    g_pActiveNetwork = this;
    m_pInstance->RunCallbacks();
}

NetClientPtr Network::AddConnection( HSteamNetConnection hConn )
{
    ZoneScoped;

    m_pInstance->SetConnectionPollGroup( hConn, m_hPollGroup );

    m_umConnectedClients.emplace( hConn,
                                  std::make_shared<NetClient>( m_pInstance, hConn ) );

    return m_umConnectedClients.at( hConn );
}

NetClientPtr Network::GetConnection( HSteamNetConnection hConn )
{
    ZoneScoped;

    auto it = m_umConnectedClients.find( hConn );
    if ( it != m_umConnectedClients.end() )
    {
        return it->second;
    }

    return nullptr;
}

void Network::DestroyConnection( HSteamNetConnection hConn )
{
    ZoneScoped;

    NetClientPtr client = GetConnection( hConn );
    if ( client == nullptr || !client->IsValid() ) return;

    // We cannot destroy connections
    // This technically creates a memory leak
    // but we dont fucking care :)
    m_umConnectedClients.erase( hConn );
}

/* static */
void Network::OnStatusChanged( SteamNetConnectionStatusChangedCallback_t *pInfo )
{
    ZoneScoped;

    switch ( pInfo->m_info.m_eState )
    {
    case k_ESteamNetworkingConnectionState_ClosedByPeer:
    case k_ESteamNetworkingConnectionState_ProblemDetectedLocally:
    {
        ZoneScoped;

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
        ZoneScoped;

        auto conn = g_pActiveNetwork->GetConnection( pInfo->m_hConn );
        if ( conn == nullptr ) conn = g_pActiveNetwork->AddConnection( pInfo->m_hConn );

        conn->OnStateChange( conn, ConnectionState::Connecting,
                             (const char *) pInfo->m_info.m_szEndDebug );

        g_pActiveNetwork->OnStateChange( conn, ConnectionState::Connecting,
                                         (const char *) pInfo->m_info.m_szEndDebug );

        break;
    }

    case k_ESteamNetworkingConnectionState_Connected:
    {
        ZoneScoped;

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