#include "ItemInfoManager.hh"

void ItemInfoManager::AddItem( Item &item )
{
    item.uID = m_vItems.size();
    m_vItems.push_back( item );
}

Item *ItemInfoManager::GetItem( uint16_t uID )
{
    if ( uID > m_vItems.size() + 1 ) return 0;

    return &m_vItems [ uID ];
}

bool ItemInfoManager::Pack( Kokoro::Memory::Buffer &buffer )
{
    m_iVersion = ITEMDB_VERSION;

    buffer.Push( m_iVersion );
    buffer.Push<uint16_t>( m_vItems.size() );

    for ( auto &item : m_vItems )
    {
        item.Pack( buffer );
    }

    return true;
}

bool ItemInfoManager::Unpack( Kokoro::Memory::Buffer &buffer )
{
    if ( !buffer.can_read( 6 ) )
    {
        return false;
    }

    m_iVersion = buffer.Pop<uint32_t>( 4 );

    auto itemAmount = buffer.Pop<uint16_t>( 2 );
    m_vItems.resize( itemAmount );

    for ( auto &item : m_vItems )
    {
        if ( !item.Unpack( m_iVersion, buffer ) ) return false;
    }

    return true;
}
