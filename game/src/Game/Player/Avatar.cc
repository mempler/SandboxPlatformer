#include "Avatar.hh"

#include "Core/Engine.hh"
#include "Core/Utils/Logger.hh"
#include "Core/Utils/Math.hh"

#include "Game/Game.hh"
#include "Game/Network/Packets/AvatarStatePacket.hh"
#include "glm/fwd.hpp"

#include <stdint.h>

void Avatar::CalculateTransform()
{
    m_m4Transform = Math::CalcTransform( m_v3Position, m_v2Size );
}

void Avatar::Tick( float fDelta )
{
    // FIXME: This might be slow for <100 avatars

    // update movement
    m_v2Velocity = {
        Math::Approach( m_v2TargetVelocity.x, m_v2Velocity.x, fDelta * g_fAcceleration ),
        Math::Approach( m_v2TargetVelocity.y, m_v2Velocity.y, fDelta * g_fAcceleration )
    };

    glm::vec2 newPosition = glm::vec2( m_v3Position ) + m_v2Velocity * fDelta;

    m_v3Position = { newPosition.x, newPosition.y, m_v3Position.z };

#if !GAME_SERVER
    CalculateTransform();
#endif

    if ( ( m_v2Velocity.x != 0.f || m_v2Velocity.y != 0.f )
         && m_MovementTimer.elapsed() >= .2f )
    {
        m_MovementTimer.reset();

        Packets::AvatarStateData data;
        data.SetAvatar( this );

        Packets::REQ_AvatarState packet {};
        packet.m_Object = &data;

        auto size = GetGame()->GetClient()->Send( packet );

#if ENGINE_DEBUG
        GetGame()->GetNetInspector()->HookSendPacket( packet.m_Header.m_eType, size );
#endif
    }
}

void Avatar::Draw()
{
    GetEngine()->GetBatcher().SubmitRectangle( 0, m_m4Transform );
}

bool Avatar::PackState( Kokoro::Memory::Buffer &buffer )
{
    ZoneScoped;

    buffer.Push( m_v3Position );
    buffer.Push( m_v2Size );
    buffer.Push( m_v2Velocity );

    return true;
}

bool Avatar::UnpackState( Kokoro::Memory::Buffer &buffer )
{
    ZoneScoped;
    if ( !buffer.can_read( 20 ) ) return false;

    m_v3Position = buffer.Pop<glm::vec3>( 12 );
    m_v2Size = buffer.Pop<glm::vec2>( 8 );
    m_v2Velocity = buffer.Pop<glm::vec2>( 8 );
    m_v2TargetVelocity = buffer.Pop<glm::vec2>( 8 );

    return true;
}
