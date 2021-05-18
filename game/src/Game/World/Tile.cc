#include "Tile.hh"

#include "Core/Engine.hh"
#include "Core/Graphics/VertexBatcher.hh"
#include "Core/Utils/Math.hh"

#include "Game/Game.hh"

#include <Tracy.hpp>

void Tile::UpdateTransform()
{
    ZoneScoped;

    m4Transform =
        Math::CalcTransform( { iPosX * 32.f, iPosY * 32.f, 5 }, { 32.f, 32.f } );
}

void Tile::Draw()
{
    ZoneScoped;

    if ( pFore )
    {
        GetEngine()->GetBatcher().SubmitWithUV(
            pFore->pAtlasTexture, m4Transform,
            { 32 * pFore->uItemX, 32 * pFore->uItemY, 32, 32 } );
    }

    if ( pBack )
    {
        GetEngine()->GetBatcher().SubmitWithUV(
            pBack->pAtlasTexture, m4Transform,
            { 32 * pBack->uItemX, 32 * pBack->uItemY, 32, 32 } );
    }
}

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

    pFore = GAME->GetItemInfoMan().GetItem( fgId );
    pBack = GAME->GetItemInfoMan().GetItem( bgId );

    iPosX = posX;
    iPosY = posY;

#if !GAME_SERVER
    UpdateTransform();
#endif

    return true;
}
