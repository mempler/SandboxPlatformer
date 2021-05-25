#pragma once

#include "Network/Packet.hh"

namespace Packets
{
    struct LoginPacket
    {
        std::string m_sName = "";
        
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

    typedef IBasePacket<PacketType::CLN_RequestLogin, LoginPacket> REQ_AvatarLogin;
    typedef IBasePacket<PacketType::SRV_SendLogin, LoginPacket> SND_AvatarLogin;
}  // namespace Packets
