#pragma once

class Avatar {
public:
    Avatar(const glm::vec3 &v3Pos, const std::string &sName);

    void OnUpdate(float fDelta);
    void OnRender();

private:
    glm::vec3 m_v3Position;
    glm::vec2 m_v2Scale;

    glm::vec2 m_v2Velocity;
    glm::vec2 m_v2TargetVelocity;
};