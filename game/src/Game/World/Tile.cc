#include "Tile.hh"

#include "Core/Engine.hh"
#include "Core/Graphics/VertexBatcher.hh"
#include "Core/Utils/Math.hh"

#include "Game/Game.hh"

#if !GAME_SERVER
void Tile::UpdateTransform()
{
    m4Transform =
        Math::CalcTransform( { iPosX * 32.f, iPosY * 32.f, 5 }, { 32.f, 32.f } );
}

void Tile::RenderForeground()
{
    if ( !pFore ) return;

    auto *atlas = GetEngine()->GetTextureManager().Load( pFore->Atlas );
    GetEngine()->GetBatcher().SubmitWithUV(
        atlas, m4Transform, { 0 * pFore->uItemX, 0 * pFore->uItemY, 32, 32 } );
}

void Tile::RenderBackground()
{
    if ( !pBack ) return;

    auto *atlas = GetEngine()->GetTextureManager().Load( pBack->Atlas );
    GetEngine()->GetBatcher().SubmitWithUV(
        atlas, m4Transform, { 0 * pBack->uItemX, 0 * pBack->uItemY, 32, 32 } );
}

void Tile::RenderTileShadow()
{
    // TODO: do stuff
}
#endif

void Tile::Pack( Kokoro::Memory::Buffer &buffer )
{
    buffer.Push<uint16_t>( pFore == nullptr ? 0 : pFore->uID );
    buffer.Push<uint16_t>( pBack == nullptr ? 0 : pBack->uID );

    buffer.Push( iPosX );
    buffer.Push( iPosY );
}

void Tile::Unpack( uint32_t iWorldVersion, Kokoro::Memory::Buffer &buffer )
{
    auto fgId = buffer.Pop<uint16_t>( 2 );
    auto bgId = buffer.Pop<uint16_t>( 2 );

    auto posX = buffer.Pop<uint16_t>( 2 );
    auto posY = buffer.Pop<uint16_t>( 2 );

    pFore = GetGame()->GetItemInfoMan().GetItem( fgId );
    pBack = GetGame()->GetItemInfoMan().GetItem( bgId );

    iPosX = posX;
    iPosY = posY;
}
