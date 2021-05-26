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
}

void Avatar::Draw()
{
    GetEngine()->GetBatcher().SubmitRectangle( 0, m_m4Transform );
}