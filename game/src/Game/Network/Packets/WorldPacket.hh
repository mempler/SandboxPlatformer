#pragma once

#include "Game/World/World.hh"

#include "Network/Packet.hh"

namespace Packets
{
    struct WorldRequestData
    {
        std::string m_sName;

        bool Pack( Kokoro::Memory::Buffer &buffer )
        {
            buffer.Push<uint8_t>( m_sName.size() );
            buffer.Push( m_sName );

            return true;
        }

        bool Unpack( Kokoro::Memory::Buffer &buffer )
        {
            if ( !buffer.can_read( 1 ) )
            {
                return false;
            }

            auto size = buffer.Pop<uint8_t>( 1 );
            if ( !buffer.can_read( size ) )
            {
                return false;
            }

            return true;
        }
    };

    typedef IBasePacket<PacketType::CLN_RequestWorld, WorldRequestData> REQ_World;
    typedef IBasePacket<PacketType::SRV_SendWorld, World> SND_World;
}  // namespace Packets
