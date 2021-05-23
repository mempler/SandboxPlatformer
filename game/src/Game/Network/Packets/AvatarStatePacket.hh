#pragma once

#include "Game/Player/Avatar.hh"

#include "Network/Packet.hh"

namespace Packets
{
    struct AvatarStatePacket
    {
        glm::vec3 m_v3Position {};
        glm::vec2 m_v2Size = {};
        glm::vec2 m_v2Velocity {};
        glm::vec2 m_v2TargetVelocity {};

        void SetAvatar( Avatar &avatar )
        {
            m_v3Position = avatar.m_v3Position;
            m_v2Size = avatar.m_v2Size;
            m_v2Velocity = avatar.m_v2Velocity;
            m_v2TargetVelocity = avatar.m_v2TargetVelocity;
        }

        bool Pack( Kokoro::Memory::Buffer &buffer )
        {
            buffer.Push( m_v3Position );
            buffer.Push( m_v2Size );
            buffer.Push( m_v2Velocity );
            buffer.Push( m_v2TargetVelocity );

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

    typedef IBasePacket<PacketType::CLN_RequestAvatarState, AvatarStatePacket>
        REQ_AvatarState;
    typedef IBasePacket<PacketType::SRV_SendAvatarState, AvatarStatePacket>
        SND_AvatarState;
}  // namespace Packets
