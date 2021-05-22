#include "Tile.hh"

#include "Core/Engine.hh"
#include "Core/Graphics/VertexBatcher.hh"
#include "Core/Utils/Math.hh"

#include "Game/Game.hh"

#include <Tracy.hpp>

void Tile::UpdateTransform()
{
    ZoneScoped;

    m4Transform = Math::CalcTransform( { iPos * 32, 5 }, { 32.f, 32.f } );
}

void Tile::Draw()
{
    ZoneScoped;

    if ( pBack && !pFore )
    {
        ZoneScopedN( "pBack->Draw()" );
        ZoneValue( pBack->uID );
        GetEngine()->GetBatcher().SubmitWithUV( pBack->pAtlasTexture, m4Transform,
                                                pBack->v4UVs );
    }
    else if ( pFore )
    {
        ZoneScopedN( "pFore->Draw()" );
        ZoneValue( pFore->uID );
        GetEngine()->GetBatcher().SubmitWithUV( pFore->pAtlasTexture, m4Transform,
                                                pFore->v4UVs );
    }
}

bool Tile::Pack( Kokoro::Memory::Buffer &buffer )
{
    ZoneScoped;

    buffer.Push<uint16_t>( pFore == nullptr ? 0 : pFore->uID );
    buffer.Push<uint16_t>( pBack == nullptr ? 0 : pBack->uID );

    buffer.Push<uint16_t>( iPos.x );
    buffer.Push<uint16_t>( iPos.y );

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

    iPos.x = posX;
    iPos.y = posY;

#if !GAME_SERVER
    UpdateTransform();
#endif

    return true;
}
