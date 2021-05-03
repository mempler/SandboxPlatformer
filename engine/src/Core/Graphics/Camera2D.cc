#include "pch.hh"

#include "Camera2D.hh"

#include "Core/Engine.hh"

Camera2D::Camera2D(const glm::vec2 &v2Pos, const glm::vec2 &v2Scale) : m_v2Pos(v2Pos), m_v2Scale(v2Scale) {
    CalculateMetrices();
}

void Camera2D::Init() {
    // I wonder what would happen if this Camera gets destroyed...
    GetEngine()->GetWindow().OnResize.connect<&Camera2D::OnResize>(this);
}

static glm::mat4 CalculateView(const glm::vec2 &v2Pos, float fRotation) {
    return glm::inverse(
        glm::translate(glm::mat4(1.f), glm::vec3(v2Pos.x, v2Pos.y, .0f)) * glm::rotate(glm::mat4(1.f), glm::radians(fRotation), glm::vec3(.0f, .0f, 1.f)));
}

static glm::mat4 CalculateProjection(const glm::vec2 &v2Scale) {
    glm::mat4 proj = glm::ortho(0.0f, v2Scale.x, v2Scale.y, .0f, .1f, 1000.f);
    proj[3].z = 1.f;
    return proj;
}

void Camera2D::CalculateMetrices() {
    m_m4Projection = CalculateProjection(m_v2Scale);
    m_m4View = CalculateView(m_v2Pos, m_fRotation);
}

void Camera2D::SetPosition(const glm::vec2 &v2Pos) {
    m_v2Pos = v2Pos;
    m_m4View = CalculateView(m_v2Pos, m_fRotation);
}

void Camera2D::SetScale(const glm::vec2 &v2Scale) {
    m_v2Scale = v2Scale;
    m_m4Projection = CalculateProjection(m_v2Scale);
}

void Camera2D::SetZoom(float fPower) {
    // Nothing, at least for now
}

void Camera2D::SetUniformTransform(bgfx::ViewId vViewID) {
    bgfx::setViewTransform(vViewID, glm::value_ptr(m_m4View), glm::value_ptr(m_m4Projection));
}

void Camera2D::OnResize(GameWindow *pGameWindow, uint32_t iWidth, uint32_t iHeight) {
    // Update Projection Matrix
    SetScale({ iWidth, iHeight });
}