#include "pch.hh"

#include "GameView.hh"

#include "Core/Engine.hh"

void GameView::Draw()
{
    if ( !bgfx::isValid( m_hFrameBuffer ) )
    {
        BaseSurface *surface = m_pEngine->GetSurface();

        m_hFrameBuffer = bgfx::createFrameBuffer(
            surface->GetWidth(), surface->GetHeight(), bgfx::TextureFormat::RGBA8 );
    }

    ImGui::Begin( "Game View", &m_bIsShowing );
    auto size = ImGui::GetWindowSize();

    if ( bgfx::isValid( m_hFrameBuffer ) )
    {
        BaseSurface *surface = m_pEngine->GetSurface();

        // Render everything to frame buffer, note that imgui is on view 999!
        bgfx::setViewFrameBuffer( 0, m_hFrameBuffer );

        glm::vec2 scale = ApplyScale(
            {
                surface->GetWidth(),
                surface->GetHeight(),
            },
            { size.x, size.y } );

        auto texture = bgfx::getTexture( m_hFrameBuffer );

        auto windowCenter = size;
        windowCenter.x -= surface->GetWidth() * scale.x * 0.6f;
        windowCenter.y -= surface->GetHeight() * scale.y * 0.6f;
        windowCenter.x *= 0.5;
        windowCenter.y *= 0.5;

        glm::vec4 UVs = { 0, 0, 1, 1 };
        if ( bgfx::getCaps()->originBottomLeft )
        {
            UVs = { 0, 1, 1, 0 };
        }

        ImGui::SetCursorPos( windowCenter );
        ImGui::Image( (void *) (intptr_t) texture.idx,
                      { surface->GetWidth() * scale.x * 0.6f,
                        surface->GetHeight() * scale.y * 0.6f },
                      { UVs.x, UVs.y }, { UVs.z, UVs.w } );
    }

    ImGui::End();

    if ( !m_bIsShowing )
    {
        // We no longer show

        bgfx::setViewFrameBuffer( 0, BGFX_INVALID_HANDLE );
    }
}