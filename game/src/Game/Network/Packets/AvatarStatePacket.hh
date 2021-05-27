#pragma once

#include "Game/Player/Avatar.hh"
#include "glm/fwd.hpp"

#include "Network/Packet.hh"

#include <glm/gtx/compatibility.hpp>

namespace Packets
{
    struct AvatarStateData
    {
        uint32_t m_ID = 0;

        glm::vec3 m_v3Position {};
        glm::vec2 m_v2Size = {};
        glm::vec2 m_v2Velocity {};
        glm::vec2 m_v2TargetVelocity {};

        void SetAvatar( Avatar *pAvatar )
        {
            m_v3Position = pAvatar->m_v3Position;
            m_v2Size = pAvatar->m_v2Size;
            m_v2Velocity = pAvatar->m_v2Velocity;
            m_v2TargetVelocity = pAvatar->m_v2TargetVelocity;
            m_ID = pAvatar->m_ID;
        }

        void InitAvatar( Avatar *pAvatar, bool bInterp = false, float fRTT = 0.f )
        {
            if ( bInterp && fRTT < 1.2f )
            {
                pAvatar->m_v3Position =
                    glm::lerp( pAvatar->m_v3Position, m_v3Position, fRTT );
            }
            else
            {
                pAvatar->m_v3Position = m_v3Position;
            }

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

            return true;
        }

        bool Unpack( Kokoro::Memory::Buffer &buffer )
        {
            if ( !buffer.can_read( 1 ) )
            {
                return false;
            }

            m_v3Position = buffer.Pop<glm::vec3>( 12 );
            m_v2Size = buffer.Pop<glm::vec2>( 8 );
            m_v2Velocity = buffer.Pop<glm::vec2>( 8 );
            m_v2TargetVelocity = buffer.Pop<glm::vec2>( 8 );
            m_ID = buffer.Pop<uint32_t>( 4 );

            return true;
        }
    };

    typedef IBasePacket<PacketType::CLN_RequestAvatarState, AvatarStateData>
        REQ_AvatarState;
    typedef IBasePacket<PacketType::SRV_SendAvatarState, AvatarStateData> SND_AvatarState;
}  // namespace Packets
