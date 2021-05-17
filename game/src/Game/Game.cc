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

    // CONNECT TO NETWORK
    SteamNetworkingIPAddr targetAddress;
    targetAddress.Clear();
    targetAddress.ParseString( "127.0.0.1:27015" );  // TODO: Don't hardcode this

    m_pFont = GetEngine()->GetFontManager().LoadFromFile( "file://Roboto-Regular.ttf",
                                                          256, 256, 22.f );
    m_pNetworkClient = m_Network.ConnectTo( targetAddress );
    m_pNetworkClient->OnStateChange.connect<&Game::OnStateChange>( this );
}

void Game::Tick( float fDeltaTime )
{
    m_World.Tick( fDeltaTime );
    m_Network.Tick();
}

void Game::Draw()
{
#if !GAME_SERVER
    m_World.Draw();

    if ( m_pNetworkClient ) m_lConnectionStatus.Render();
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

void Game::OnStateChange( BaseClientPtr pClient, ConnectionState eState,
                          const char *szMessage )
{
    switch ( eState )
    {
    case ConnectionState::Disconnected:
        m_lConnectionStatus.SetText( { 0, 0, 999.f },
                                     "Network: Not Connected... Retrying", m_pFont );
        m_lConnectionStatus.SetColor( { 1, .3, .3, 1 } );

        SteamNetworkingIPAddr targetAddress;
        targetAddress.Clear();
        targetAddress.ParseString( "127.0.0.1:27015" );  // TODO: Don't hardcode this

        m_pNetworkClient = m_Network.ConnectTo( targetAddress );
        m_pNetworkClient->OnStateChange.connect<&Game::OnStateChange>( this );
        break;
    case ConnectionState::Connected:
        m_lConnectionStatus.SetText( { 0, 0, 999.f }, "Network: Connected!", m_pFont );
        m_lConnectionStatus.SetColor( { .3, 1, .3, 1 } );
        break;

    case ConnectionState::Connecting:
        m_lConnectionStatus.SetText( { 0, 0, 999.f }, "Network: Connecting...", m_pFont );
        m_lConnectionStatus.SetColor( { 1, 1, .3, 1 } );
        break;
    default: break;
    }
}