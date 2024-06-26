#pragma once

#include <cstdint>

#include "Core/Utils/Logger.hh"

#include <Kokoro/Memory/Buffer.hh>
#include <enet/enet.h>

#include <Tracy.hpp>

enum class PacketType : uint16_t
{
    CLN_RequestItemDB,
    SRV_SendItemDB,

    CLN_RequestWorld,
    SRV_SendWorld,

    CLN_RequestAvatar,
    SRV_SendAvatar,

    CLN_RequestAvatarState,
    SRV_SendAvatarState,
    
    CLN_RequestLogin,
    SRV_SendLogin,
};

// TODO, this is not engine stuff
constexpr const char *PacketType_ToString( PacketType eType )
{
    const char *Type = "";
    switch ( eType )
    {
    case PacketType::CLN_RequestItemDB: Type = "RequestItemDB"; break;
    case PacketType::SRV_SendItemDB: Type = "SendItemDB"; break;

    case PacketType::CLN_RequestWorld: Type = "RequestWorld"; break;
    case PacketType::SRV_SendWorld: Type = "SendWorld"; break;

    case PacketType::CLN_RequestAvatar: Type = "RequestAvatar"; break;
    case PacketType::SRV_SendAvatar: Type = "SendAvatar"; break;

    default: Type = "Unknown"; break;
    }

    return Type;
}

enum class PacketFlags : uint16_t
{
    None = 0
};

struct PacketHeader
{
    PacketType m_eType;
    PacketFlags m_eFlags;
    uint8_t m_vPadding [ 28 ];  // In case we want to add smth in the header

    bool Pack( Kokoro::Memory::Buffer &buffer )
    {
        ZoneScoped;

        buffer.Push( m_eType );
        buffer.Push( m_eFlags );
        buffer.Append( m_vPadding );

        return true;
    }

    bool Unpack( Kokoro::Memory::Buffer &buffer )
    {
        ZoneScoped;

        if ( !buffer.can_read( sizeof( PacketHeader ) ) )
        {
            return false;
        }

        m_eType = buffer.Pop<PacketType>( 2 );
        m_eFlags = buffer.Pop<PacketFlags>( 2 );

        // Nasty hack
        auto tmpData = buffer.Pop<const char *>( sizeof( m_vPadding ) );
        // fix your stl fucking hell
        // std::copy( buffer.current(), buffer.current() + sizeof( m_vPadding ),
        //            m_vPadding );
        delete tmpData;

        return true;
    }
};

template <PacketType eType, typename T>
struct IBasePacket
{
    PacketHeader m_Header { eType, PacketFlags::None };
    T *m_Object = nullptr;

    bool Pack( Kokoro::Memory::Buffer &buffer )
    {
        ZoneScoped;

        m_Header.Pack( buffer );

        return m_Object->Pack( buffer );
    }

    bool Unpack( Kokoro::Memory::Buffer &buffer )
    {
        ZoneScoped;

        // m_Header.Unpack( buffer );

        return m_Object->Unpack( buffer );
    }

    size_t SendTo( ENetPeer *hConn, bool bReliable = true )
    {
        ZoneScoped;

        Kokoro::Memory::Buffer buffer;

        if ( !Pack( buffer ) )
        {
            Console::Error( "Failed to send packet!" );
        }

        auto data = buffer.data();
        auto size = buffer.size();

        // Send buffer to client
        ENetPacket *packet = enet_packet_create(
            data, size,
            ( bReliable ) ? ENET_PACKET_FLAG_RELIABLE : ENET_PACKET_FLAG_UNSEQUENCED );

        enet_peer_send( hConn, 0, packet );

        return size;
    }
};
