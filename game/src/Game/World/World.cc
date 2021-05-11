#include "World.hh"

#include "Core/Engine.hh"
#include "Core/Utils/Math.hh"

#include "Game/Game.hh"
#include "Game/World/WorldRenderer.hh"

#include "bgfx/bgfx.h"

#define XYTV(x, y) (x + (y * m_uWidth))

constexpr uintptr_t g_uFrameBufferFlags =
    BGFX_TEXTURE_RT | BGFX_SAMPLER_MIN_POINT | BGFX_SAMPLER_MAG_POINT | BGFX_SAMPLER_MIP_POINT | BGFX_SAMPLER_U_CLAMP | BGFX_SAMPLER_V_CLAMP;

void World::Init(uint16_t uWidth, uint16_t uHeight) {
    m_uWidth = uWidth;
    m_uHeight = uHeight;

    m_vTiles.resize(uWidth * uHeight);
}

void World::Tick(float fDeltaTime) {
}

void World::Draw() {
    GameWindow &window = GetEngine()->GetWindow();

    if (bgfx::isValid(m_hWorldFrameBuffer))
        bgfx::destroy(m_hWorldFrameBuffer);

    m_hWorldFrameBuffer = bgfx::createFrameBuffer(window.Width(), window.Height(), bgfx::TextureFormat::RGBA8, g_uFrameBufferFlags);

    GetEngine()->GetBatcher().Reset();
    GetEngine()->GetBatcher().SetCurrentView(2);

    bgfx::setViewFrameBuffer(2, m_hWorldFrameBuffer);

    WorldRenderer::Draw(this);

    GetEngine()->GetBatcher().Reset();

    GetEngine()->GetBatcher().SubmitRectangleRawHandle(bgfx::getTexture(m_hWorldFrameBuffer), Math::CalcTransform({ 0, 0, 1 }, { window.Width(), window.Height() }));

    GetEngine()->GetBatcher().SetCurrentView(0);
}

void World::PlaceFore(uint16_t uID, uint16_t x, uint16_t y) {
    Tile *tile = &m_vTiles[XYTV(x, y)];

    tile->v3Pos = { x * 32.f, y * 32.f, 5 };
    tile->v2Size = { 32.f, 32.f };
    tile->UpdateTransform();
    tile->pFore = GetGame()->GetItemInfoMan().GetItem(uID);
}

void World::PlaceBack(uint16_t uID, uint16_t x, uint16_t y) {
}