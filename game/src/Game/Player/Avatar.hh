#pragma once

const float g_fVelX = 300.f;
const float g_fVelY = 250.f;
const float g_fAcceleration = 1500.f;

class Player;
class Avatar {
public:
    Avatar(const glm::vec3 &v3Pos, const std::string &sName);

    void OnUpdate(float fDelta);
    void OnRender();

    // transformation utilities
    void SetPosition(const glm::vec3 &v3Pos);
    void SetSize(const glm::vec2 &v2Size);
    void SetTargetVel(const glm::vec2 &v2TVel);

public:
    const glm::vec2 &GetTargetVelocity() const {
        return m_v2TargetVelocity;
    }

private:
    glm::vec3 m_v3Position{};
    glm::vec2 m_v2Size = { 20.f, 30.f };
    glm::mat4 m_m4Transform{};

    glm::vec2 m_v2Velocity{ 500, 0 };
    glm::vec2 m_v2TargetVelocity{ 10, 0 };

    friend Player;
};