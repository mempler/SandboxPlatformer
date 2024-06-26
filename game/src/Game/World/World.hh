#pragma once

#include "Tile.hh"

#include "Game/Player/Avatar.hh"

#include <Kokoro/Memory/Buffer.hh>

#define WORLD_VERSION 0

enum class WorldState : uint32_t
{
    None = 0,
    IsValid = 1 << 1,
};

BitFlags( WorldState );

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

    void AvatarOnEnter( Avatar *pAvatar );
    void AvatarOnLeave( Avatar *pAvatar );

    // Network stuff
    bool Pack( Kokoro::Memory::Buffer &buffer );
    bool Unpack( Kokoro::Memory::Buffer &buffer );

    std::vector<Avatar *> m_vAvatars;

  public:
    bool IsValid()
    {
        return ( m_eState & WorldState::IsValid );
    }

    bgfx::FrameBufferHandle &GetFrameBuffer()
    {
        return m_hWorldFrameBuffer;
    }

  private:
    void RenderAvatars();

  private:
    uint16_t m_uVersion = WORLD_VERSION;

    uint16_t m_uWidth;
    uint16_t m_uHeight;

    WorldState m_eState = WorldState::None;

    bgfx::FrameBufferHandle m_hWorldFrameBuffer = BGFX_INVALID_HANDLE;

  protected:
    std::vector<Tile> m_vTiles;
};