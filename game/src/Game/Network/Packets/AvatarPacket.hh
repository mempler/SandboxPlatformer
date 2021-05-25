#pragma once

#include "Game/Player/Avatar.hh"

#include "Network/Packet.hh"

namespace Packets
{
    struct AvatarReceiveData
    {
        uint32_t m_ID = 0;

        glm::vec3 m_v3Position {};
        glm::vec2 m_v2Size = {};
        glm::vec2 m_v2Velocity {};
        glm::vec2 m_v2TargetVelocity {};

        bool m_bLocal = false;

        void SetAvatar( Avatar *pAvatar, bool bLocal )
        {
            m_v3Position = pAvatar->m_v3Position;
            m_v2Size = pAvatar->m_v2Size;
            m_v2Velocity = pAvatar->m_v2Velocity;
            m_v2TargetVelocity = pAvatar->m_v2TargetVelocity;
            m_bLocal = bLocal;
            m_ID = pAvatar->m_ID;
        }

        void InitAvatar( Avatar *pAvatar )
        {
            pAvatar->m_v3Position = m_v3Position;
            pAvatar->m_v2Size = m_v2Size;
            pAvatar->m_v2Velocity = m_v2Velocity;
            pAvatar->m_v2TargetVelocity = m_v2TargetVelocity;
            pAvatar->m_ID = m_ID;
        }

        bool Pack( Kokoro::Memory::Buffer &buffer )
        {
            buffer.Push( m_v3Position );
            buffer.Push( m_v2Size );
            buffer.Push( m_v2Velocity );
            buffer.Push( m_v2TargetVelocity );
            buffer.Push( m_ID );  // m_ID
            buffer.Push( m_bLocal );

            return true;
        }

        bool Unpack( Kokoro::Memory::Buffer &buffer )
        {
            if ( !buffer.can_read( 30 ) )
            {
                return false;
            }

            m_v3Position = buffer.Pop<glm::vec3>( 12 );
            m_v2Size = buffer.Pop<glm::vec2>( 8 );
            m_v2Velocity = buffer.Pop<glm::vec2>( 8 );
            m_v2TargetVelocity = buffer.Pop<glm::vec2>( 8 );
            m_ID = buffer.Pop<uint32_t>( 4 );
            m_bLocal = buffer.Pop<bool>( 1 );

            return true;
        }
    };

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
    typedef IBasePacket<PacketType::SRV_SendAvatar, AvatarReceiveData> SND_Avatar;
}  // namespace Packets
