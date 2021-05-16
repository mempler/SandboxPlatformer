#pragma once

#include "Tile.hh"

#include "Game/Player/Avatar.hh"

#include "WorldRenderer.hh"

#include <Kokoro/Memory/Buffer.hh>

#define WORLD_VERSION 0

enum class eWorldState
{
    None = 0,
    IsValid = 1 << 1,
};

EnumFlags( eWorldState );

class World
{
  public:
    World() {};
    ~World() {};

    void Init( uint16_t uWidth, uint16_t uHeight );
    void Tick( float fDeltaTime );
    void Draw();

    void PlaceFore( uint16_t uID, uint16_t x, uint16_t y );
    void PlaceBack( uint16_t uID, uint16_t x, uint16_t y );

    // Events
    Avatar *AddAvatar( Avatar *avatar );

#if !GAME_SERVER
    void OnPlayerEnter();
#endif

    // Network stuff
    void Pack( Kokoro::Memory::Buffer &buffer )
    {
        m_iWorldVersion = WORLD_VERSION;
        buffer.Push( m_iWorldVersion );

        buffer.Push( m_uWidth );
        buffer.Push( m_uHeight );
        buffer.Push( m_eState );

        for ( auto &tile : m_vTiles )
        {
            tile.Pack( buffer );
        }
    }

    void Unpack( Kokoro::Memory::Buffer &buffer )
    {
        m_iWorldVersion = buffer.Pop<uint16_t>( 2 );

        m_uWidth = buffer.Pop<uint16_t>( 2 );
        m_uHeight = buffer.Pop<uint16_t>( 2 );

        m_vTiles.resize( m_uWidth * m_uHeight );

        for ( auto &tile : m_vTiles )
        {
            tile.Unpack( m_iWorldVersion, buffer );
        }
    }

  public:
    bool IsValid()
    {
        return ( m_eState & eWorldState::IsValid );
    }

#if !GAME_SERVER
    bgfx::FrameBufferHandle &GetFrameBuffer()
    {
        return m_hWorldFrameBuffer;
    }
#endif

  private:
#if !GAME_SERVER
    void RenderAvatars();
#endif

  private:
    std::vector<Avatar *> m_vAvatars;

    uint16_t m_iWorldVersion = WORLD_VERSION;

    uint16_t m_uWidth;
    uint16_t m_uHeight;

    eWorldState m_eState = eWorldState::None;

#if !GAME_SERVER
    bgfx::FrameBufferHandle m_hWorldFrameBuffer = BGFX_INVALID_HANDLE;
#endif

  protected:
    std::vector<Tile> m_vTiles;

    friend class WorldRenderer;
};