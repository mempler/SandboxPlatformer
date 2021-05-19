#pragma once

#include "Core/Engine.hh"
#include "Core/Graphics/Texture2D.hh"
#include "Core/Utils/Identifier.hh"

#include <Kokoro/Memory/Buffer.hh>
#include <glm/glm.hpp>

#include <Tracy.hpp>

#define ITEMDB_VERSION 0

struct Item
{
    uint16_t uID = 0;

    uint16_t uItemX;
    uint16_t uItemY;

    Identifier Atlas = EmptyIdentifier;

    // Game stuff
    Texture2D *pAtlasTexture = nullptr;
    glm::vec4 v4UVs = { 0, 0, 32, 32 };

    // Network stuff
    bool Pack( Kokoro::Memory::Buffer &buffer )
    {
        ZoneScoped;

        buffer.Push( uID );
        buffer.Push( uItemX );
        buffer.Push( uItemY );

        buffer.Push<uint32_t>( Atlas.Raw().size() );
        buffer.Push<const char *>( Atlas.Raw().data() );

        return true;
    }

    bool Unpack( uint32_t iItemDBVersion, Kokoro::Memory::Buffer &buffer )
    {
        ZoneScoped;

        if ( !buffer.can_read( 10 ) )
        {
            return false;
        }

        uID = buffer.Pop<uint16_t>( 2 );
        uItemX = buffer.Pop<uint16_t>( 2 );
        uItemY = buffer.Pop<uint16_t>( 2 );

        size_t atlasSize = buffer.Pop<uint32_t>( 4 );
        if ( !buffer.can_read( atlasSize ) )
        {
            return false;
        }

        auto heapStr = buffer.Pop<const char *>( atlasSize );
        Atlas = heapStr;
        delete [] heapStr;

#if !GAME_SERVER
        pAtlasTexture = GetEngine()->GetTextureManager().Load( Atlas );

        v4UVs = { 32 * uItemX, 32 * uItemY, 32, 32 };
#endif

        return true;
    }
};

class ItemInfoManager
{
  public:
    ItemInfoManager() {};
    ~ItemInfoManager() {};

    void AddItem( Item &item );
    Item *GetItem( uint16_t uID );

    // Network stuff
    bool Pack( Kokoro::Memory::Buffer &buffer );
    bool Unpack( Kokoro::Memory::Buffer &buffer );

  private:
    uint32_t m_iVersion = ITEMDB_VERSION;

    std::vector<Item> m_vItems;
};
