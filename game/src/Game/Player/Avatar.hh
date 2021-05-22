#pragma once

#include <glm/glm.hpp>

constexpr float g_fVelX = 300.f;
constexpr float g_fVelY = 250.f;
constexpr float g_fAcceleration = 5000.f;

class Player;
struct Avatar
{
    glm::vec3 m_v3Position {};
    glm::vec2 m_v2Size = { 20.f, 30.f };

    glm::vec2 m_v2Velocity { 0, 0 };
    glm::vec2 m_v2TargetVelocity { 0, 0 };

    glm::mat4 m_m4Transform {};

    void CalculateTransform();

    void Tick( float fDelta );
    void Draw();
};
