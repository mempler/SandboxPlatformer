#include "Game.hh"

#include "Core/Engine.hh"

void Game::OnResolutionChanged( BaseSurface *pSurface, uint32_t iWidth, uint32_t iHeight )
{
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
    // PREINIT EVENTS
    GetEngine()->GetSurface()->OnResolutionChanged.connect<&Game::OnResolutionChanged>(
        this );
    GetEngine()->GetInputManager().OnKeyDown.connect<&Player::OnKeyDown>( &m_Player );
    GetEngine()->GetInputManager().OnKeyRelease.connect<&Player::OnKeyRelease>(
        &m_Player );

    // PREINIT VIEWS
    GetEngine()->AddView( 2 );  // World tile layer
    GetEngine()->ResetTransform();

    // PRELOAD ITEMS, FOR NOW
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
}

void Game::Tick( float fDeltaTime )
{
    m_World.Tick( fDeltaTime );
}

void Game::Draw()
{
    m_World.Draw();
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