#pragma once

const float g_fVelX = 500.f;
const float g_fVelY = 250.f;
const float g_fAcceleration = 50.f;

class Player;
class Avatar
{
  public:
    Avatar( const glm::vec3 &v3Pos, const std::string &sName );

    void OnUpdate( float fDelta );
    void OnRender();

    // transformation utilities
    void SetPosition( const glm::vec3 &v3Pos );
    void SetSize( const glm::vec2 &v2Size );
    void SetTargetVel( const glm::vec2 &v2TVel );

  public:
    const glm::vec2 &GetTargetVelocity() const
    {
        return m_v2TargetVelocity;
    }

  private:
    glm::vec3 m_v3Position {};
    glm::vec2 m_v2Size = { 20.f, 30.f };
    glm::mat4 m_m4Transform {};

    glm::vec2 m_v2Velocity { 0, 0 };
    glm::vec2 m_v2TargetVelocity { 0, 0 };

    friend Player;
};