#pragma once

#include "Game/Player/Avatar.hh"

#include "Network/Packet.hh"

namespace Packets
{
    struct AvatarRequestData
    {
        bool Pack( Kokoro::Memory::Buffer &buffer )
        {
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

    typedef IBasePacket<PacketType::CLN_RequestAvatar, AvatarRequestData> REQ_Avatar;
    typedef IBasePacket<PacketType::SRV_SendAvatar, Avatar> SND_Avatar;
}  // namespace Packets
