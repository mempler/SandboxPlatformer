#include "World.hh"

#include "Game/Game.hh"
#include "Game/World/WorldRenderer.hh"

#define XYTV(x, y) (x + (y * m_uWidth))

void World::Init(uint16_t uWidth, uint16_t uHeight) {
    m_uWidth = uWidth;
    m_uHeight = uHeight;
    
    m_vTiles.resize(uWidth * uHeight);
}

void World::Tick(float fDeltaTime) {
}

void World::Draw() {
    WorldRenderer::Draw(this);
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