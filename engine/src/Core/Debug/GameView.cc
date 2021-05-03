#include "pch.hh"

#include "GameView.hh"

#include "Core/Engine.hh"
#include "Core/Graphics/Window.hh"

#include <Core/Utils/Math.hh>

#include <cstdint>

void GameView::Draw() {
    if (!bgfx::isValid(m_hFrameBuffer)) {
        GameWindow &window = m_pEngine->GetWindow();

        m_hFrameBuffer = bgfx::createFrameBuffer(window.Width(), window.Height(), bgfx::TextureFormat::RGBA8);
    }

    ImGui::Begin("Game View", &m_bIsShowing);
    auto size = ImGui::GetWindowSize();

    if (bgfx::isValid(m_hFrameBuffer)) {
        GameWindow &window = m_pEngine->GetWindow();

        // Render everything to frame buffer, note that imgui is on view 999!
        bgfx::setViewFrameBuffer(0, m_hFrameBuffer);

        glm::vec2 scale = ApplyScale({ window.Width(), window.Height() }, { size.x, size.y });

        auto texture = bgfx::getTexture(m_hFrameBuffer);

        auto windowCenter = size;
        windowCenter.x -= window.Width() * scale.x * 0.6f;
        windowCenter.y -= window.Height() * scale.y * 0.6f;
        windowCenter.x *= 0.5;
        windowCenter.y *= 0.5;

        ImGui::SetCursorPos(windowCenter);
        ImGui::Image((void *)(intptr_t)texture.idx, { window.Width() * scale.x * 0.6f, window.Height() * scale.y * 0.6f }, { 0, 1 }, { 1, 0 });
    }

    ImGui::End();

    if (!m_bIsShowing) {
        // We no longer show

        bgfx::setViewFrameBuffer(0, BGFX_INVALID_HANDLE);
    }
}