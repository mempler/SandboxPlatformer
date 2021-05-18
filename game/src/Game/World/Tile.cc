#include "Tile.hh"

#include "Core/Engine.hh"
#include "Core/Graphics/VertexBatcher.hh"
#include "Core/Utils/Math.hh"

#include "Game/Game.hh"

#include <Tracy.hpp>

#if !GAME_SERVER
void Tile::UpdateTransform()
{
    ZoneScoped;

    m4Transform =
        Math::CalcTransform( { iPosX * 32.f, iPosY * 32.f, 5 }, { 32.f, 32.f } );
}

void Tile::RenderForeground()
{
    ZoneScoped;

    if ( !pFore ) return;

    if ( !pAtlasFore )
    {
        pAtlasFore = GetEngine()->GetTextureManager().Load( pFore->Atlas );
    }

    GetEngine()->GetBatcher().SubmitWithUV(
        pAtlasFore, m4Transform, { 0 * pFore->uItemX, 0 * pFore->uItemY, 32, 32 } );
}

void Tile::RenderBackground()
{
    ZoneScoped;

    if ( !pBack ) return;

    if ( !pAtlasBack )
    {
        pAtlasBack = GetEngine()->GetTextureManager().Load( pBack->Atlas );
    }

    GetEngine()->GetBatcher().SubmitWithUV(
        pAtlasBack, m4Transform, { 0 * pBack->uItemX, 0 * pBack->uItemY, 32, 32 } );
}

void Tile::RenderTileShadow()
{
    // TODO: do stuff
}
#endif

bool Tile::Pack( Kokoro::Memory::Buffer &buffer )
{
    ZoneScoped;

    buffer.Push<uint16_t>( pFore == nullptr ? 0 : pFore->uID );
    buffer.Push<uint16_t>( pBack == nullptr ? 0 : pBack->uID );

    buffer.Push( iPosX );
    buffer.Push( iPosY );

    return true;
}

bool Tile::Unpack( uint32_t iWorldVersion, Kokoro::Memory::Buffer &buffer )
{
    ZoneScoped;

    if ( !buffer.can_read( 8 ) )
    {
        return false;
    }

    auto fgId = buffer.Pop<uint16_t>( 2 );
    auto bgId = buffer.Pop<uint16_t>( 2 );

    auto posX = buffer.Pop<uint16_t>( 2 );
    auto posY = buffer.Pop<uint16_t>( 2 );

    pFore = GetGame()->GetItemInfoMan().GetItem( fgId );
    pBack = GetGame()->GetItemInfoMan().GetItem( bgId );

    iPosX = posX;
    iPosY = posY;

    return true;
}
