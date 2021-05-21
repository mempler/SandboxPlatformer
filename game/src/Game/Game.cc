#include "Game.hh"

#include "Core/Engine.hh"

#include "Game/Network/Packets/ItemDBPacket.hh"
#include "Game/Network/Packets/WorldPacket.hh"

void Game::OnResolutionChanged( BaseSurface *pSurface, uint32_t iWidth, uint32_t iHeight )
{
    ZoneScoped;

    if ( m_World.IsValid() )
    {
        if ( bgfx::isValid( m_World.GetFrameBuffer() ) )
            bgfx::destroy( m_World.GetFrameBuffer() );

        m_World.GetFrameBuffer() = bgfx::createFrameBuffer(
            iWidth, iHeight, bgfx::TextureFormat::RGBA8, g_uFrameBufferFlags );
    }
}

Game::Game() : m_ItemInfoManager(), m_World(), m_Player()
{
}

Game::~Game()
{
}

void Game::Init()
{
    ZoneScoped;

    // PREINIT EVENTS
    GetEngine()->GetSurface()->OnResolutionChanged.connect<&Game::OnResolutionChanged>(
        this );
    GetEngine()->GetInputManager().OnKeyDown.connect<&Player::OnKeyDown>( &m_Player );
    GetEngine()->GetInputManager().OnKeyRelease.connect<&Player::OnKeyRelease>(
        &m_Player );

#if ENGINE_DEBUG
    m_pNetworkInspector =
        GetEngine()->RegisterDebugUtil<NetworkInspector>( true, "Network" );
#endif

    // PREINIT VIEWS
    GetEngine()->AddView( 2 );  // World tile layer
    GetEngine()->ResetTransform();

    // CONNECT TO NETWORK
    ENetAddress address = { 0 };
    enet_address_set_host( &address, "127.0.0.1" );
    address.port = 27015;  // FIXME: Don't hardcode this

    m_pFont = GetEngine()->GetFontManager().LoadFromFile( "file://Roboto-Regular.ttf",
                                                          256, 256, 22.f );
    m_pNetworkClient = m_Network.ConnectTo( address );
    
    m_pNetworkClient->OnStateChange.connect<&Game::OnStateChange>( this );
    m_pNetworkClient->OnPacket.connect<&Game::OnPacket>( this );
}

void Game::Tick( float fDeltaTime )
{
    ZoneScoped;

    m_World.Tick( fDeltaTime );
    m_Network.Tick();
}

void Game::Draw()
{
    ZoneScoped;

    m_World.Draw();

    if ( m_pNetworkClient ) m_lConnectionStatus.Render();
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
    ZoneScoped;

    Packets::WorldRequestData data;
    data.m_sName = svName;

    Packets::REQ_World packet {};
    packet.m_Object = &data;

    auto size = m_pNetworkClient->Send( packet );

#if ENGINE_DEBUG
    m_pNetworkInspector->HookSendPacket( packet.m_Header.m_eType, size );
#endif
}

void Game::RequestItemDB()
{
    ZoneScoped;

    Packets::ItemDBRequestData data;

    Packets::REQ_ItemDB packet {};
    packet.m_Object = &data;

    auto size = m_pNetworkClient->Send( packet );

#if ENGINE_DEBUG
    m_pNetworkInspector->HookSendPacket( packet.m_Header.m_eType, size );
#endif
}

void Game::OnStateChange( NetClientPtr pClient, ConnectionState eState,
                          const char *szMessage )
{
    ZoneScoped;

    switch ( eState )
    {
    case ConnectionState::Disconnected:
    {
        m_lConnectionStatus.SetText( { 0, 0, 999.f },
                                     "Network: Not Connected... Retrying", m_pFont );
        m_lConnectionStatus.SetColor( { 1, .3, .3, 1 } );

        ENetAddress address = { 0 };
        enet_address_set_host( &address, "127.0.0.1" );
        address.port = 27015;

        m_pNetworkClient = m_Network.ConnectTo( address );
        m_pNetworkClient->OnStateChange.connect<&Game::OnStateChange>( this );
        break;
    }
    case ConnectionState::Connected:
        m_lConnectionStatus.SetText( { 0, 0, 999.f }, "Network: Connected!", m_pFont );
        m_lConnectionStatus.SetColor( { .3, 1, .3, 1 } );

        // Before we can request any worlds, we gotta get an ItemDB
        RequestItemDB();
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
    ZoneScoped;

#if ENGINE_DEBUG
    m_pNetworkInspector->HookRecievePacket( header.m_eType, buffer.size() );
#endif

    switch ( header.m_eType )
    {
    case PacketType::SRV_SendItemDB:
    {
        if ( !m_ItemInfoManager.Unpack( buffer ) )
        {
            Console::Error( "Failed to unpack ItemDB!" );
        }

        // FIXME: Move this somewhere else
        // Request the world "START"
        RequestWorld( "START" );

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
