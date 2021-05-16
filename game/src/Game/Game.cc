#include "Game.hh"

#include "steam/isteamnetworkingutils.h"

#include "Core/Engine.hh"

#if !GAME_SERVER
void Game::OnGameResize( GameWindow *pGameWindow, uint32_t iWidth, uint32_t iHeight )
{
    if ( m_World.IsValid() )
    {
        if ( bgfx::isValid( m_World.GetFrameBuffer() ) )
            bgfx::destroy( m_World.GetFrameBuffer() );

        m_World.GetFrameBuffer() =
            bgfx::createFrameBuffer( pGameWindow->Width(), pGameWindow->Height(),
                                     bgfx::TextureFormat::RGBA8, g_uFrameBufferFlags );
    }
}
#endif

Game::Game() : m_ItemInfoManager(), m_World(), m_Player()
{
}

Game::~Game()
{
}

void Game::Init()
{
    // PREINIT EVENTS
#if !GAME_SERVER
    GetEngine()->GetWindow().OnResize.connect<&Game::OnGameResize>( this );
    GetEngine()->GetInputManager().OnKeyDown.connect<&Player::OnKeyDown>( &m_Player );
    GetEngine()->GetInputManager().OnKeyRelease.connect<&Player::OnKeyRelease>(
        &m_Player );
#endif

    // PREINIT VIEWS
    GetEngine()->GetWindow().AddView( 2 );  // World tile layer
    GetEngine()->GetWindow().ResetTransform();

    // PREINIT NETWORK
    SteamDatagramErrMsg errMsg;
    if ( !GameNetworkingSockets_Init( nullptr, errMsg ) )
        Console::Fatal( "GameNetworkingSockets_Init failed: {}", errMsg );

    m_pSteamSockets = SteamNetworkingSockets();

    // CONNECT TO NETWORK
    SteamNetworkingIPAddr targetAddress;
    targetAddress.Clear();
    targetAddress.ParseString( "127.0.0.1:27015" );

    SteamNetworkingConfigValue_t opt;
    opt.SetPtr( k_ESteamNetworkingConfig_Callback_ConnectionStatusChanged,
                (void *) &Game::OnStatusChanged );

    m_hConnection = m_pSteamSockets->ConnectByIPAddress( targetAddress, 1, &opt );

    // PRELOAD ITEMS, FOR NOW
    /*
    // Rock
    m_ItemInfoManager.Preload( { 0., 0, 32, 32 },
                               GetEngine()->GetTextureManager().CreateTextureFromFile(
                                   "file://tiles1.png", TEXTURE_FORMAT_NEAREST ) );
    // Dirt
    m_ItemInfoManager.Preload( { 32, 0, 32, 32 },
                               GetEngine()->GetTextureManager().CreateTextureFromFile(
                                   "file://tiles1.png", TEXTURE_FORMAT_NEAREST ) );
    // Grass
    m_ItemInfoManager.Preload( { 64, 0, 32, 32 },
                               GetEngine()->GetTextureManager().CreateTextureFromFile(
                                   "file://tiles1.png", TEXTURE_FORMAT_NEAREST ) );

    m_World.Init( 100, 60 );

    for ( uint16_t i = 0; i < 100; i++ ) m_World.PlaceFore( 2, i, 0 );

    for ( uint16_t x = 0; x < 100; x++ )
    {
        for ( uint16_t y = 1; y < 60; y++ )
        {
            m_World.PlaceFore( 1, x, y );
        }
    }

    m_World.OnPlayerEnter();
    */
}

void Game::Tick( float fDeltaTime )
{
    m_pSteamSockets->RunCallbacks();

    m_World.Tick( fDeltaTime );
}

void Game::Draw()
{
#if !GAME_SERVER
    m_World.Draw();
#endif
}

ItemInfoManager &Game ::GetItemInfoMan()
{
    return m_ItemInfoManager;
}

World &Game::GetWorld()
{
    return m_World;
}

Player &Game::GetLocalPlayer()
{
    return m_Player;
}

/* static */
void Game::OnStatusChanged( SteamNetConnectionStatusChangedCallback_t *pInfo )
{
    Game *game = GetGame();

    switch ( pInfo->m_info.m_eState )
    {
    case k_ESteamNetworkingConnectionState_ClosedByPeer:
    case k_ESteamNetworkingConnectionState_ProblemDetectedLocally:
    {
        Console::Error( "We disconnected... ({})", pInfo->m_info.m_szEndDebug );

        game->m_pSteamSockets->CloseConnection( pInfo->m_hConn, 0, nullptr, false );
        break;
    }

    case k_ESteamNetworkingConnectionState_Connecting:
    {
        Console::Info( "We're trying to establish an connection! ({})",
                       pInfo->m_info.m_szEndDebug );
        break;
    }

    case k_ESteamNetworkingConnectionState_Connected:
    {
        Console::Info( "We established an connection! ({})", pInfo->m_info.m_szEndDebug );
        break;
    }

    default: break; /* Unhandled */
    }
}
