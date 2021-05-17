#include "Server.hh"

#include "Ascii.h"

#include "Core/Utils/Limiter.hh"
#include "Core/Utils/Logger.hh"
#include "Core/Utils/Timer.hh"

#include "Kokoro/Memory/Buffer.hh"

#define TICK_SPEED 60

void Server::Init()
{
    Console::Init();

    Console::Info( "This server is running \r\n{}", g_ASCII );
    Console::Info( "Running: IceServer" );
    Console::Info( "Version: {}", "1.0.0" );

    SteamDatagramErrMsg errMsg;
    if ( !GameNetworkingSockets_Init( nullptr, errMsg ) )
        Console::Fatal( "GameNetworkingSockets_Init failed: {}", errMsg );

    // PREINIT
    m_Network.OnStateChange.connect<&Server::OnStateChange>( this );

    SteamNetworkingIPAddr address;
    address.Clear();
    address.m_port = 27015;

    m_Listener = m_Network.CreateListener( address );
    Console::Info( "Server listening on port {}", address.m_port );

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

    for ( uint16_t i = 0; i < 100; i++ ) m_World.PlaceFore( 2, i, 0 );
    for ( uint16_t x = 0; x < 100; x++ )
    {
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
        Console::Trace( "World ({}) -> Peer", "test" );  // TODO: world name

        break;
    }
    case ConnectionState::Connecting:
    {
        Console::Info( "A new peer is connecting!" );

        // A client is attempting to connect
        // Try to accept the connection.
        if ( m_Listener.AcceptConnection( pClient ) )
        {
            pClient->Disconnect( "Unexpected closure!!" );
            Console::Error( "Couldn't accept connection. (Why?)" );
            break;
        }

        break;
    }
    case ConnectionState::Disconnected:
    {
        Console::Info( "A peer disconnected... ({})", szMessage );
        break;
    }
    }
}
