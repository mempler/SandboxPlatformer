#include <array>

#include "Network.hh"

#include <Tracy.hpp>

#include <enet.h>

static Network *g_pActiveNetwork;

Network::Network()
{
    ZoneScoped;

    if ( enet_initialize() != 0 ) Console::Fatal( "Failed to initialize ENet!" );
}

NetClientPtr Network::ConnectTo( ENetAddress &address )
{
    ZoneScoped;
}

void Network::Tick()
{
    ZoneScoped;

    // Accept messages
    // ISteamNetworkingMessage *pIncomingMsg = nullptr;
    // m_pInstance->ReceiveMessagesOnPollGroup( m_hPollGroup, &pIncomingMsg, 1 );

    // if ( pIncomingMsg != nullptr )
    // {
    //     Kokoro::Memory::Buffer buffer( (uint8_t *) pIncomingMsg->GetData(),
    //                                    pIncomingMsg->GetSize() );

    //     PacketHeader header;
    //     if ( !header.Unpack( buffer ) )
    //     {
    //         Console::Error( "Invalid message! skipping..." );
    //     }
    //     else
    //     {
    //         NetClientPtr client = GetConnection( pIncomingMsg->GetConnection() );

    //         pIncomingMsg->Release();  // Release resources we dont need it anymore

    //         OnPacket( client, header,
    //                   buffer );  // Let our signal subscriber handle all this.
    //         client->OnPacket( client, header, buffer );
    //     }
    // }

    // g_pActiveNetwork = this;
    // m_pInstance->RunCallbacks();
}

/* static */
void Network::OnStatusChanged()
{
    ZoneScoped;

    // switch ( pInfo->m_info.m_eState )
    // {
    // case k_ESteamNetworkingConnectionState_ClosedByPeer:
    // case k_ESteamNetworkingConnectionState_ProblemDetectedLocally:
    // {
    //     ZoneScoped;

    //     auto conn = g_pActiveNetwork->GetConnection( pInfo->m_hConn );
    //     conn->OnStateChange( conn, ConnectionState::Disconnected,
    //                          (const char *) pInfo->m_info.m_szEndDebug );

    //     g_pActiveNetwork->OnStateChange( conn, ConnectionState::Disconnected,
    //                                      (const char *) pInfo->m_info.m_szEndDebug );

    //     conn->Disconnect( pInfo->m_info.m_szEndDebug );
    //     g_pActiveNetwork->DestroyConnection( pInfo->m_hConn );
    //     break;
    // }

    // case k_ESteamNetworkingConnectionState_Connecting:
    // {
    //     ZoneScoped;

    //     auto conn = g_pActiveNetwork->GetConnection( pInfo->m_hConn );
    //     if ( conn == nullptr ) conn = g_pActiveNetwork->AddConnection( pInfo->m_hConn
    //     );

    //     conn->OnStateChange( conn, ConnectionState::Connecting,
    //                          (const char *) pInfo->m_info.m_szEndDebug );

    //     g_pActiveNetwork->OnStateChange( conn, ConnectionState::Connecting,
    //                                      (const char *) pInfo->m_info.m_szEndDebug );

    //     break;
    // }

    // case k_ESteamNetworkingConnectionState_Connected:
    // {
    //     ZoneScoped;

    //     auto conn = g_pActiveNetwork->GetConnection( pInfo->m_hConn );
    //     conn->OnStateChange( conn, ConnectionState::Connected,
    //                          (const char *) pInfo->m_info.m_szEndDebug );

    //     g_pActiveNetwork->OnStateChange( conn, ConnectionState::Connected,
    //                                      (const char *) pInfo->m_info.m_szEndDebug );
    //     break;
    // }

    // default: break; /* Unhandled */
    // }
}