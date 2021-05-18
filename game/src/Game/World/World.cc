#include "World.hh"

#include "Server/Server.hh"

#include "Core/Engine.hh"
#include "Core/Utils/Math.hh"

#include "Game/Game.hh"
#include "Game/Player/Avatar.hh"

#include <Tracy.hpp>

#define XYTV( x, y ) ( x + ( y * m_uWidth ) )

void World::Init( uint16_t uWidth, uint16_t uHeight )
{
    ZoneScoped;

    m_uWidth = uWidth;
    m_uHeight = uHeight;

    m_vTiles.resize( uWidth * uHeight );

    m_eState |= eWorldState::IsValid;  // testing purposes

#if !GAME_SERVER
    GameWindow &window = GetEngine()->GetWindow();

    // TODO: MOVE THIS TO A BETTER PLACE
    if ( bgfx::isValid( m_hWorldFrameBuffer ) ) bgfx::destroy( m_hWorldFrameBuffer );

    m_hWorldFrameBuffer =
        bgfx::createFrameBuffer( window.Width(), window.Height(),
                                 bgfx::TextureFormat::RGBA8, g_uFrameBufferFlags );
#endif
}

void World::Tick( float fDeltaTime )
{
    ZoneScoped;

    if ( !IsValid() ) return;

    for ( auto &avatar : m_vAvatars ) avatar.Tick( fDeltaTime );
}

void World::Draw()
{
    ZoneScoped;

    if ( !IsValid() ) return;

    GameWindow &window = GetEngine()->GetWindow();

    GetEngine()
        ->GetBatcher()
        .Reset();  // reset what we've drawn, we will be switching views
    GetEngine()->GetBatcher().SetCurrentView( 2 );
    bgfx::setViewFrameBuffer(
        2,
        m_hWorldFrameBuffer );  // tell bgfx that we are using this framebuffer

    // Render out the world
    for ( auto &tile : m_vTiles )
    {
        if ( tile.pFore == nullptr && tile.pBack == nullptr ) continue;

        tile.Draw();
    }

    RenderAvatars();

    GetEngine()->GetBatcher().Reset();  // we are done
    GetEngine()->GetBatcher().SubmitRectangleRawHandle(
        bgfx::getTexture( m_hWorldFrameBuffer ),
        Math::CalcTransform( { 0, 0, 1 }, { window.Width(), window.Height() } ) );
    GetEngine()->GetBatcher().SetCurrentView( 0 );  // back to default view
}

void World::PlaceFore( uint16_t uID, uint16_t x, uint16_t y )
{
    ZoneScoped;

    Tile *tile = &m_vTiles [ XYTV( x, y ) ];

    tile->iPosX = x;
    tile->iPosX = y;

#if !GAME_SERVER  // This is too expensive for the server
    tile->UpdateTransform();
#endif

    tile->pFore = GAME->GetItemInfoMan().GetItem( uID );
}

void World::PlaceBack( uint16_t uID, uint16_t x, uint16_t y )
{
    ZoneScoped;
}

Avatar *World::CreateAvatar()
{
    ZoneScoped;

    return &m_vAvatars.emplace_back();
}

bool World::Pack( Kokoro::Memory::Buffer &buffer )
{
    ZoneScoped;

    m_iWorldVersion = WORLD_VERSION;
    buffer.Push( m_iWorldVersion );

    buffer.Push( m_uWidth );
    buffer.Push( m_uHeight );
    buffer.Push( m_eState );

    for ( auto &tile : m_vTiles )
    {
        tile.Pack( buffer );
    }

    return true;
}

bool World::Unpack( Kokoro::Memory::Buffer &buffer )
{
    ZoneScoped;

    if ( !buffer.can_read( 10 ) )
    {
        return false;
    }

    Init( 0, 0 );  // Init ourself

    m_iWorldVersion = buffer.Pop<uint16_t>( 2 );

    m_uWidth = buffer.Pop<uint16_t>( 2 );
    m_uHeight = buffer.Pop<uint16_t>( 2 );
    m_eState = buffer.Pop<eWorldState>( 4 );

    m_vTiles.resize( m_uWidth * m_uHeight );

    for ( auto &tile : m_vTiles )
    {
        if ( !tile.Unpack( m_iWorldVersion, buffer ) ) return false;
    }

    return true;
}

void World::RenderAvatars()
{
    ZoneScoped;

    for ( auto &avatar : m_vAvatars ) avatar.Draw();
}
