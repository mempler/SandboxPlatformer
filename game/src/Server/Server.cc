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

    m_pSteamSockets = SteamNetworkingSockets();

    SteamNetworkingIPAddr serverLocalAddr;
    serverLocalAddr.Clear();
    serverLocalAddr.m_port = 27015;

    SteamNetworkingConfigValue_t opt;
    opt.SetPtr( k_ESteamNetworkingConfig_Callback_ConnectionStatusChanged,
                (void *) &Server::OnStatusChanged );

    m_hSocket = m_pSteamSockets->CreateListenSocketIP( serverLocalAddr, 1, &opt );
    if ( m_hSocket == k_HSteamListenSocket_Invalid )
    {
        Console::Fatal( "Failed to listen on port {}", 27015 );
    }

    m_hPollGroup = m_pSteamSockets->CreatePollGroup();
    if ( m_hPollGroup == k_HSteamNetPollGroup_Invalid )
    {
        Console::Fatal( "Failed to listen on port {}", 27015 );
    }

    Console::Info( "Server listening on port {}", 27015 );

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
    m_pSteamSockets->RunCallbacks();
}

/* static */
void Server::OnStatusChanged( SteamNetConnectionStatusChangedCallback_t *pInfo )
{
    Server *server = GetServer();

    switch ( pInfo->m_info.m_eState )
    {
    case k_ESteamNetworkingConnectionState_ClosedByPeer:
    case k_ESteamNetworkingConnectionState_ProblemDetectedLocally:
    {
        Console::Info( "A peer disconnected... ({})", pInfo->m_info.m_szEndDebug );

        server->m_pSteamSockets->CloseConnection( pInfo->m_hConn, 0, nullptr, false );
        break;
    }

    case k_ESteamNetworkingConnectionState_Connecting:
    {
        Console::Info( "A new peer is connecting!" );

        // A client is attempting to connect
        // Try to accept the connection.
        if ( server->m_pSteamSockets->AcceptConnection( pInfo->m_hConn ) != k_EResultOK )
        {
            server->m_pSteamSockets->CloseConnection( pInfo->m_hConn, 0, nullptr, false );

            Console::Error( "Couldn't accept connection. (Why?)" );
            break;
        }

        // Assign the poll group
        if ( !server->m_pSteamSockets->SetConnectionPollGroup( pInfo->m_hConn,
                                                               server->m_hPollGroup ) )
        {
            server->m_pSteamSockets->CloseConnection( pInfo->m_hConn, 0, nullptr, false );

            Console::Error( "Couldn't set poll group?" );
            break;
        }

        break;
    }

    case k_ESteamNetworkingConnectionState_Connected:
    {
        Console::Info( "A peer has established a connection!" );
        Console::Trace( "World ({}) -> Peer", "test" );  // TODO: world name

        break;
    }

    default: break; /* Unhandled */
    }
}
