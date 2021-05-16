#include "World.hh"

#include "Server/Server.hh"

#include "Core/Engine.hh"
#include "Core/Utils/Math.hh"

#include "Game/Game.hh"
#include "Game/Player/Avatar.hh"
#include "Game/World/WorldRenderer.hh"


#define XYTV( x, y ) ( x + ( y * m_uWidth ) )

void World::Init( uint16_t uWidth, uint16_t uHeight )
{
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
    if ( !IsValid() ) return;

    for ( auto &&i : m_vAvatars ) i->OnUpdate( fDeltaTime );
}

#if !GAME_SERVER
void World::Draw()
{
    if ( !IsValid() ) return;

    GameWindow &window = GetEngine()->GetWindow();

    GetEngine()
        ->GetBatcher()
        .Reset();  // reset what we've drawn, we will be switching views
    GetEngine()->GetBatcher().SetCurrentView( 2 );
    bgfx::setViewFrameBuffer(
        2,
        m_hWorldFrameBuffer );  // tell bgfx that we are using this framebuffer

    WorldRenderer::Draw( this );  // WorldRenderer will handle what we want

    RenderAvatars();

    GetEngine()->GetBatcher().Reset();  // we are done
    GetEngine()->GetBatcher().SubmitRectangleRawHandle(
        bgfx::getTexture( m_hWorldFrameBuffer ),
        Math::CalcTransform( { 0, 0, 1 }, { window.Width(), window.Height() } ) );
    GetEngine()->GetBatcher().SetCurrentView( 0 );  // back to default view
}

void World::RenderAvatars()
{
    for ( auto &&i : m_vAvatars ) i->OnRender();
}
#endif

void World::PlaceFore( uint16_t uID, uint16_t x, uint16_t y )
{
    Tile *tile = &m_vTiles [ XYTV( x, y ) ];

    tile->iPosX = x;
    tile->iPosX = y;

#if !GAME_SERVER
    tile->UpdateTransform();
#endif

#if !GAME_SERVER
    tile->pFore = GetGame()->GetItemInfoMan().GetItem( uID );
#else
    tile->pFore = GetServer()->GetItemInfoMan().GetItem( uID );
#endif
}

void World::PlaceBack( uint16_t uID, uint16_t x, uint16_t y )
{
}

Avatar *World::AddAvatar( Avatar *avatar )
{
    m_vAvatars.push_back( avatar );

    return avatar;
}

#if !GAME_SERVER
void World::OnPlayerEnter()
{
    Avatar *avatar = new Avatar( { 100.f, 100.f, 7 }, "None" );
    GetGame()->GetLocalPlayer().InitAvatar( AddAvatar( avatar ) );
}
#endif
