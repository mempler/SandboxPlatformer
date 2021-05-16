#include "WorldRenderer.hh"

#include "World.hh"

void WorldRenderer::Draw( World *pWorld )
{
    for ( auto &tile : pWorld->m_vTiles ) tile.RenderForeground();

    for ( auto &tile : pWorld->m_vTiles ) tile.RenderBackground();
}