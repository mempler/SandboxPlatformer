#include <array>

#include "Network.hh"

#include "enet/enet.h"

#include "Core/Utils/Logger.hh"

#include <Tracy.hpp>

static Network *g_pActiveNetwork;

Network::Network()
{
    ZoneScoped;

    if ( enet_initialize() != 0 ) Console::Fatal( "Failed to initialize ENet!" );

    m_pInstance = enet_host_create( 0, 1, 2, 0, 0 );

    if ( !m_pInstance )
    {
        Console::Fatal( "Failed to initialize ENet host!" );
    }
}

NetClientPtr Network::ConnectTo( ENetAddress &address )
{
    ZoneScoped;

    ENetPeer *peer = enet_host_connect( m_pInstance, &address, 2, 0 );
    if ( !peer )
    {
        Console::Fatal( "Failed to initialize ENet peer!" );
    }

    return std::make_shared<NetClient>( m_pInstance, peer );
}

void Network::Tick()
{
    ZoneScoped;

    // Accept messages
    ENetEvent event {};
    while ( enet_host_service( m_pInstance, &event, 0 ) > 0 )
    {
        switch ( event.type )
        {
        case ENET_EVENT_TYPE_CONNECT: Console::Info( "Peer connected." ); break;
        case ENET_EVENT_TYPE_RECEIVE:
        {
            Kokoro::Memory::Buffer buffer( event.packet->data, event.packet->dataLength );

            PacketHeader header;
            if ( !header.Unpack( buffer ) )
            {
                Console::Error( "Invalid message! skipping..." );
            }
            else
            {
                // Let our signal subscriber handle all this.
            }

            enet_packet_destroy( event.packet );
            break;
        }
        case ENET_EVENT_TYPE_DISCONNECT: Console::Info( "Peer disconnected." ); break;
        default: break;
        }
    }
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