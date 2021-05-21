#include "Server.hh"

#include "Ascii.h"

#include "Core/Utils/Limiter.hh"
#include "Core/Utils/Logger.hh"
#include "Core/Utils/Timer.hh"

#include "Game/Network/Packet.hh"
#include "Game/Network/Packets/ItemDBPacket.hh"
#include "Game/Network/Packets/WorldPacket.hh"

#include "Kokoro/Memory/Buffer.hh"

#include <enet/enet.h>

#define TICK_SPEED 60

void Server::Init()
{
    Console::Init();

    Console::Info( "This server is running \r\n{}", g_ASCII );
    Console::Info( "Running: IceServer" );
    Console::Info( "Version: {}", "1.0.0" );

    if ( enet_initialize() != 0 ) Console::Fatal( "Failed to initialize ENet!" );

    // PREINIT
    m_Network.OnStateChange.connect<&Server::OnStateChange>( this );
    m_Network.OnPacket.connect<&Server::OnPacket>( this );

    ENetAddress address = { 0 };
    address.host = ENET_HOST_ANY;
    address.port = 27015;

    Console::Info( "Server listening on port {}", address.port );

    // TODO: actual world loading & items.dat
    Console::Info( "Loading items.dat..." );

    Item rock;
    rock.uItemX = 0;
    rock.uItemY = 0;
    rock.Atlas = "file://tiles1.png";
    m_ItemInfoManager.AddItem( rock );

    Item dirt;
    dirt.uItemX = 1;
    dirt.uItemY = 0;
    dirt.Atlas = "file://tiles1.png";
    m_ItemInfoManager.AddItem( dirt );

    Item grass;
    grass.uItemX = 2;
    grass.uItemY = 0;
    grass.Atlas = "file://tiles1.png";
    m_ItemInfoManager.AddItem( grass );

    Console::Info( "Creating an empty world..." );

    m_World.Init( 100, 60 );

    for ( uint16_t x = 0; x < 100; x++ )
    {
        m_World.PlaceFore( 2, x, 0 );
        for ( uint16_t y = 1; y < 60; y++ )
        {
            m_World.PlaceFore( 1, x, y );
        }
    }

    Console::Info( "Done!" );
}

void Server::Run()
{
    Init();

    Timer timer;
    FrameLimiter<TICK_SPEED> limiter;
    while ( true )
    {
        double elapsed = timer.elapsed();

        Tick( elapsed );

        timer.reset();
        limiter.Wait();
    }
}

void Server::Tick( float fDeltaTime )
{
    // Poll messages

    m_Network.Tick();
}

void Server::OnStateChange( NetClientPtr pClient, ConnectionState eState,
                            const char *szMessage )
{
    switch ( eState )
    {
    case ConnectionState::Connected:
    {
        Console::Info( "A peer has established a connection!" );

        break;
    }
    case ConnectionState::Connecting:
    {
        Console::Info( "A new peer is connecting!" );

        // A client is attempting to connect
        // Try to accept the connection.
        // if ( m_Listener.AcceptConnection( pClient ) )
        // {
        //     pClient->Disconnect( "Unexpected closure!!" );
        //     Console::Error( "Couldn't accept connection. (Why?)" );
        //     break;
        // }

        break;
    }
    case ConnectionState::Disconnected:
    {
        Console::Info( "A peer disconnected... ({})", szMessage );
        break;
    }
    }
}

void Server::OnPacket( NetClientPtr pClient, PacketHeader header,
                       Kokoro::Memory::Buffer buffer )
{
    switch ( header.m_eType )
    {
    case PacketType::CLN_RequestItemDB:
    {
        Console::Trace( "A peer requested the ItemDB to be send" );

        // TODO: load itemdb from a file
        Packets::SND_ItemDB packet;
        packet.m_Object = &m_ItemInfoManager;

        pClient->Send( packet );
        break;
    }

    case PacketType::CLN_RequestWorld:
    {
        Console::Trace( "A peer requested the world to be send" );

        // TODO: load worlds from a file
        Packets::SND_World packet;
        packet.m_Object = &m_World;

        pClient->Send( packet );
        break;
    }

    default:
    {
        Console::Warn( "Unimplemented Packet: {}!", (int) header.m_eType );
        break;
    }
    }
}
