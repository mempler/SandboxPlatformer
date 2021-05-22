#pragma once

#include "Core/Graphics/Surface/Surface.hh"

#if PLATFORM_WIN32

class Win32Surface : public BaseSurface
{
  public:
    Win32Surface( SurfaceDesc &desc );
    ~Win32Surface();

    void Poll() override;

    void SetTitle( const std::string &sTitle ) override;
    void SetPosition( const glm::ivec2 &ivPos ) override;
    void SetResolution( const glm::ivec2 &ivRes ) override;

    glm::ivec2 &GetCursorPosition() override;
    void SetCursor( SurfaceCursor eCursor ) override;
    void SetCursorPosition( const glm::ivec2 &ivPos ) override;

    int GetMonitorWidth() override;
    int GetMonitorHeight() override;

    bgfx::PlatformData GetPlatformData() override;

  private:
    static LRESULT CALLBACK WindowProc( HWND hwnd, UINT msg, WPARAM wParam,
                                        LPARAM lParam );

    HWND m_Handle;
    HINSTANCE m_Instance;
};

#endif