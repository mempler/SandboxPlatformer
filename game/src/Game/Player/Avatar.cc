#include "Avatar.hh"

#include "Core/Engine.hh"
#include "Core/Utils/Logger.hh"
#include "Core/Utils/Math.hh"

Avatar::Avatar( const glm::vec3 &v3Pos, const std::string &sName ) :
    m_v3Position( v3Pos )
{
}

void Avatar::OnUpdate( float fDelta )
{
    // update movement
    m_v2Velocity = { Math::Approach( m_v2TargetVelocity.x, m_v2Velocity.x,
                                     fDelta * g_fAcceleration ),
                     Math::Approach( m_v2TargetVelocity.y, m_v2Velocity.y,
                                     fDelta * g_fAcceleration ) };

    glm::vec2 newPosition = glm::vec2( m_v3Position ) + m_v2Velocity * fDelta;
    SetPosition( { newPosition.x, newPosition.y, m_v3Position.z } );
}

void Avatar::OnRender()
{
    GetEngine()->GetBatcher().SubmitRectangle( 0, m_m4Transform );
}

void Avatar::SetPosition( const glm::vec3 &v3Pos )
{
    m_v3Position = v3Pos;
    m_m4Transform = Math::CalcTransform( m_v3Position, m_v2Size );
}

void Avatar::SetSize( const glm::vec2 &v2Size )
{
}

void Avatar::SetTargetVel( const glm::vec2 &v2TVel )
{
    m_v2TargetVelocity = v2TVel;
}