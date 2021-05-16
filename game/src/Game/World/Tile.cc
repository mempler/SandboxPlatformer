#include "Tile.hh"

#include "Core/Engine.hh"
#include "Core/Graphics/VertexBatcher.hh"
#include "Core/Utils/Math.hh"

void Tile::UpdateTransform()
{
    m4Transform = Math::CalcTransform( v3Pos, v2Size );
}

void Tile::RenderForeground()
{
    if ( !pFore ) return;

    GetEngine()->GetBatcher().SubmitWithUV( pFore->pAtlas, m4Transform,
                                            pFore->v4UVs );
}

void Tile::RenderBackground()
{
    if ( !pBack ) return;

    GetEngine()->GetBatcher().SubmitWithUV( pBack->pAtlas, m4Transform,
                                            pBack->v4UVs );
}

void Tile::RenderTileShadow()
{
}