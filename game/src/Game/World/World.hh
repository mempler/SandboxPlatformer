#pragma once

#include "pch.hh"

#include "Tile.hh"

#include "WorldRenderer.hh"
#include "bgfx/bgfx.h"

enum class eWorldState {
    None = 0,
    IsValid = 1 << 1,
};

EnumFlags(eWorldState);

class World {
public:
    World(){};
    ~World(){};

    void Init(uint16_t uWidth, uint16_t uHeight);
    void Tick(float fDeltaTime);
    void Draw();

    void PlaceFore(uint16_t uID, uint16_t x, uint16_t y);
    void PlaceBack(uint16_t uID, uint16_t x, uint16_t y);

    // Events
    void OnPlayerEnter();

public:
    signals::signal<void(float)> OnAvatarUpdate;
    signals::signal<void()> OnAvatarRender;

public:
    bool IsValid() {
        return (m_eState & eWorldState::IsValid);
    }

    bgfx::FrameBufferHandle &GetFrameBuffer() {
        return m_hWorldFrameBuffer;
    }

private:
    uint16_t m_uWidth;
    uint16_t m_uHeight;

    eWorldState m_eState = eWorldState::None;

    bgfx::FrameBufferHandle m_hWorldFrameBuffer = BGFX_INVALID_HANDLE;

protected:
    std::vector<Tile> m_vTiles;

    friend class WorldRenderer;
};