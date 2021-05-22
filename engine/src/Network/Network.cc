#include <array>

#include "Network.hh"

#include "enet/enet.h"

#include "Core/Utils/Logger.hh"

#include "Network/NetClient.hh"

#include <Tracy.hpp>

static Network *g_pActiveNetwork;

Network::Network()
{
    ZoneScoped;

    if ( enet_initialize() != 0 ) Console::Fatal( "Failed to initialize ENet!" );
}

void Network::InitClient()
{
    m_pInstance = enet_host_create( 0, 1, 2, 0, 0 );

    if ( !m_pInstance )
    {
        Console::Fatal( "Failed to initialize ENet host!" );
    }
}

void Network::InitServer( ENetAddress &address, uint32_t uMaxConnection )
{
    ZoneScoped;

    m_pInstance = enet_host_create( &address, uMaxConnection, 2, 0, 0 );

    if ( !m_pInstance )
    {
        Console::Fatal( "Failed to initialize ENet host!" );
    }
}

NetClientPtr Network::ConnectTo( ENetAddress &address )
{
    ZoneScoped;

    m_bIsClient = true;

    ENetPeer *peer = enet_host_connect( m_pInstance, &address, 2, 0 );
    if ( !peer )
    {
        Console::Fatal( "Failed to initialize ENet peer!" );
    }

    return AddPeer( peer );
}

NetClientPtr Network::AddPeer( ENetPeer *peer )
{
    NetClientPtr pNetClient = new NetClient( m_pInstance, peer );
    peer->data = pNetClient;

    m_vClients.push_back( pNetClient );

    OnStateChange( pNetClient, ConnectionState::Connecting );
    pNetClient->OnStateChange( pNetClient, ConnectionState::Connecting );

    return pNetClient;
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
        case ENET_EVENT_TYPE_CONNECT:
        {
            NetClientPtr pClient;
            if ( !m_bIsClient )
                pClient = AddPeer( event.peer );
            else
                pClient = (NetClientPtr) event.peer->data;

            OnStateChange( pClient, ConnectionState::Connected );
            pClient->OnStateChange( pClient, ConnectionState::Connected );

            Console::Info( "Peer connected." );
            break;
        }
        case ENET_EVENT_TYPE_RECEIVE:
        {
            NetClientPtr pClient = (NetClientPtr) event.peer->data;
            Kokoro::Memory::Buffer buffer( event.packet->data, event.packet->dataLength );

            PacketHeader header;
            if ( !header.Unpack( buffer ) )
            {
                Console::Error( "Invalid message! skipping..." );
            }
            else
            {
                // Let our signal subscriber handle all this.
                OnPacket( pClient, header, buffer );
                pClient->OnPacket( pClient, header, buffer );
            }

            enet_packet_destroy( event.packet );
            break;
        }
        case ENET_EVENT_TYPE_DISCONNECT:
        {
            NetClientPtr pClient = (NetClientPtr) event.peer->data;
            if ( pClient == nullptr ) continue;

            OnStateChange( pClient, ConnectionState::Disconnected );
            pClient->OnStateChange( pClient, ConnectionState::Disconnected );

            for ( size_t i = 0; i < m_vClients.size(); i++ )
            {
                if ( m_vClients.at( i ) != pClient ) continue;

                m_vClients.erase( m_vClients.begin() + i );
                break;
            }

            delete pClient;

            Console::Info( "Peer disconnected." );
            break;
        }
        default: break;
        }
    }
}