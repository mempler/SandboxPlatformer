#include "WorldRenderer.hh"

#include "World.hh"

#include <Tracy.hpp>

#if !GAME_SERVER
void WorldRenderer::Draw( World *pWorld )
{
    ZoneScoped;

    for ( auto &tile : pWorld->m_vTiles )
    {
        if ( tile.pFore == nullptr && tile.pBack == nullptr ) continue;
        if ( tile.pFore != nullptr )
        {
            tile.RenderForeground();
        }
        if ( tile.pFore == nullptr )
        {
            tile.RenderBackground();
        }
    }
}
#endif
