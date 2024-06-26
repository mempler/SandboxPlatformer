#include "Game.hh"

#include "Core/Engine.hh"
#include "Core/Utils/Logger.hh"

#include "Game/Network/Packets/AvatarPacket.hh"
#include "Game/Network/Packets/AvatarStatePacket.hh"
#include "Game/Network/Packets/ItemDBPacket.hh"
#include "Game/Network/Packets/WorldPacket.hh"
#include "Game/Player/Avatar.hh"

#include "Network/Packet.hh"

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

    // PREINIT RESOURCES
    m_pFont = GetEngine()->GetFontManager().LoadFromFile(
        "file://Fonts/Roboto-Regular.ttf", 256, 256, 22.f );

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

    m_Network.InitClient();

    m_pNetworkClient = m_Network.ConnectTo( address );

    m_pNetworkClient->OnStateChange.connect<&Game::OnStateChange>( this );
    m_pNetworkClient->OnPacket.connect<&Game::OnPacket>( this );
}

void Game::Tick( float fDeltaTime )
{
    ZoneScoped;

    m_World.Tick( fDeltaTime );
    m_Player.Tick( fDeltaTime );
    m_Network.Tick( fDeltaTime );
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
void Game::RequestAvatars()
{
    ZoneScoped;

    Packets::AvatarRequestData data;

    Packets::REQ_Avatar packet {};
    packet.m_Object = &data;

    auto size = m_pNetworkClient->Send( packet );

#if ENGINE_DEBUG
    m_pNetworkInspector->HookSendPacket( packet.m_Header.m_eType, size );
#endif
}

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

void Game::OnStateChange( NetClientPtr pClient, ConnectionState eState )
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
                     Kokoro::Memory::Buffer buffer, float fDeltaTime )
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

        // everything is ready and we want local avatar now
        RequestAvatars();

        break;
    }

    case PacketType::SRV_SendAvatar:
    {
        Packets::AvatarReceiveData data;
        if ( !data.Unpack( buffer ) )
        {
            Console::Error( "Something went wrong, server sent invalid avatar!" );
            break;
        }

        Console::Log( "Received an avatar." );

        Avatar *avatar = new Avatar;

        data.InitAvatar( avatar );

        if ( data.m_bLocal )
        {
            m_Player.InitAvatar( avatar );
        }

        m_World.AvatarOnEnter( avatar );

        break;
    }

    case PacketType::SRV_SendAvatarState:
    {
        Packets::AvatarStateData data;
        if ( !data.Unpack( buffer ) )
        {
            Console::Error( "Something went wrong, server sent invalid avatar state!" );
            break;
        }

        for ( auto avatar : m_World.m_vAvatars )
        {
            if ( avatar->m_ID == data.m_ID )
            {
                Console::Log( "DT: {}", fDeltaTime * 1000.f );
                data.InitAvatar( avatar, true, ( fDeltaTime * 1000.f ) / 200.f );
            }
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
