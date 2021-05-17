#pragma once

#include "Core/Utils/Logger.hh"

#include <Kokoro/Memory/Buffer.hh>
#include <steam/isteamnetworkingsockets.h>

#include <cstdint>

enum class PacketType : uint16_t
{
    CLN_RequestItemDB,
    SRV_SendItemDB,

    CLN_RequestWorld,
    SRV_SendWorld,
};

enum class PacketFlags : uint16_t
{
    None = 0
};

struct PacketHeader
{
    PacketType m_uType;
    PacketFlags m_eFlags;
    uint8_t m_vPadding [ 28 ];  // In case we want to add smth in the header

    bool Pack( Kokoro::Memory::Buffer &buffer )
    {
        buffer.Push( m_uType );
        buffer.Push( m_eFlags );
        buffer.Append( m_vPadding );

        return true;
    }

    bool Unpack( Kokoro::Memory::Buffer &buffer )
    {
        if ( !buffer.can_read( sizeof( PacketHeader ) ) )
        {
            return false;
        }

        m_uType = buffer.Pop<PacketType>( 2 );
        m_eFlags = buffer.Pop<PacketFlags>( 2 );

        // Nasty hack
        auto tmpData = buffer.Pop<const char *>( sizeof( m_vPadding ) );
        std::copy( buffer.current(), buffer.current() + sizeof( m_vPadding ),
                   m_vPadding );
        delete tmpData;

        return true;
    }
};

template <PacketType eType, typename T>

struct IBasePacket
{
    PacketHeader m_Header { eType, PacketFlags::None };
    T m_Object {};

    bool Pack( Kokoro::Memory::Buffer &buffer )
    {
        m_Header.Pack( buffer );

        return m_Object.Pack( buffer );
    }

    bool Unpack( Kokoro::Memory::Buffer &buffer )
    {
        m_Header.Unpack( buffer );

        return m_Object.Unpack( buffer );
    }

    size_t SendTo( ISteamNetworkingSockets *pSteamSockets, HSteamNetConnection &hConn )
    {
        Kokoro::Memory::Buffer buffer;

        if ( !Pack( buffer ) )
        {
            Console::Error( "Failed to send packet!" );
        }

        auto data = buffer.data();
        auto size = buffer.size();

        // Send buffer to client
        pSteamSockets->SendMessageToConnection( hConn, buffer.data(), buffer.size(),
                                                k_nSteamNetworkingSend_Reliable,
                                                nullptr );

        return size;
    }
};

// Helper function for getting the packet type as a string
constexpr const char *PacketType_ToString( PacketType eType )
{
    const char *Type = "";
    switch ( eType )
    {
    case PacketType::CLN_RequestItemDB: Type = "RequestItemDB"; break;
    case PacketType::SRV_SendItemDB: Type = "SendItemDB"; break;

    case PacketType::CLN_RequestWorld: Type = "RequestWorld"; break;
    case PacketType::SRV_SendWorld: Type = "SendWorld"; break;

    default: Type = "Unknown"; break;
    }

    return Type;
}
