#pragma once

#include "Core/Utils/Timer.hh"

#include "Kokoro/Memory/Buffer.hh"

#include <glm/glm.hpp>
#include <stdint.h>


constexpr float g_fVelX = 300.f;
constexpr float g_fVelY = 250.f;
constexpr float g_fAcceleration = 5000.f;

class Player;
struct Avatar
{
    std::string m_sName = "";

    glm::vec3 m_v3Position {};
    glm::vec2 m_v2Size = { 20.f, 30.f };

    glm::vec2 m_v2Velocity { 500, 0 };
    glm::vec2 m_v2TargetVelocity { 0, 0 };

    glm::mat4 m_m4Transform {};

    uint32_t m_ID = 0;

    Timer m_MovementTimer{};

    void CalculateTransform();

    void Tick( float fDelta );
    void Draw();

    // Networking
    // This should be never used i think
    bool PackState( Kokoro::Memory::Buffer &buffer );
    bool UnpackState( Kokoro::Memory::Buffer &buffer );
};
