#include "Game.hh"

#include "steam/isteamnetworkingutils.h"

#include "Core/Engine.hh"

#include "Game/Debug/NetworkInspector.hh"
#include "Game/Network/Packets/ItemDBPacket.hh"
#include "Game/Network/Packets/WorldPacket.hh"

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
#if !GAME_SERVER
    // PREINIT EVENTS
    GetEngine()->GetWindow().OnResize.connect<&Game::OnGameResize>( this );
    GetEngine()->GetInputManager().OnKeyDown.connect<&Player::OnKeyDown>( &m_Player );
    GetEngine()->GetInputManager().OnKeyRelease.connect<&Player::OnKeyRelease>(
        &m_Player );
#endif

#if ENGINE_DEBUG
    m_pNetworkInspector =
        GetEngine()->RegisterDebugUtil<NetworkInspector>( true, "Network" );
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
    m_pNetworkClient->OnPacket.connect<&Game::OnPacket>( this );
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

// Network stuff
void Game::RequestWorld( const std::string_view &svName )
{
    Packets::WorldRequestData data;
    data.m_sName = svName;

    Packets::REQ_World packet {};
    packet.m_Object = &data;

    auto size = m_pNetworkClient->Send( packet );

    m_pNetworkInspector->HookSendPacket( packet.m_Header.m_eType, size );
}

void Game::RequestItemDB()
{
    Packets::ItemDBRequestData data;

    Packets::REQ_ItemDB packet {};
    packet.m_Object = &data;

    auto size = m_pNetworkClient->Send( packet );

    m_pNetworkInspector->HookSendPacket( packet.m_Header.m_eType, size );
}

void Game::OnStateChange( NetClientPtr pClient, ConnectionState eState,
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

        // Before we can request any worlds, we gotta get an ItemDB
        RequestItemDB();

        // Request world
        RequestWorld( "START" );
        break;

    case ConnectionState::Connecting:
        m_lConnectionStatus.SetText( { 0, 0, 999.f }, "Network: Connecting...", m_pFont );
        m_lConnectionStatus.SetColor( { 1, 1, .3, 1 } );
        break;
    default: break;
    }

#if ENGINE_DEBUG
    m_pNetworkInspector->HookConnectionState( eState );
#endif
}

void Game::OnPacket( NetClientPtr pClient, PacketHeader header,
                     Kokoro::Memory::Buffer buffer )
{
    m_pNetworkInspector->HookRecievePacket( header.m_eType, buffer.size() );

    switch ( header.m_eType )
    {
    case PacketType::SRV_SendItemDB:
    {
        if ( !m_ItemInfoManager.Unpack( buffer ) )
        {
            Console::Error( "Failed to unpack ItemDB!" );
        }

        break;
    }

    case PacketType::SRV_SendWorld:
    {
        if ( !m_World.Unpack( buffer ) )
        {
            Console::Error( "Failed to unpack World!" );
        }

        break;
    }

    default:
    {
        Console::Warn( "Unimplemented Packet: {}!", (int) header.m_eType );
        break;
    }
    }
}