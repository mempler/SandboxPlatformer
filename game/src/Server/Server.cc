#include "Server.hh"

#include "Ascii.h"

#include "Core/Utils/Limiter.hh"
#include "Core/Utils/Logger.hh"
#include "Core/Utils/Timer.hh"

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

void Server::OnStatusChanged( SteamNetConnectionStatusChangedCallback_t *pStatus )
{
}