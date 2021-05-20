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

    const glm::ivec2 &GetMousePos() const override
    {
        return m_Desc.ivPos;
    }

    int GetMonitorWidth() override;
    int GetMonitorHeight() override;

  private:
    static LRESULT CALLBACK WindowProc( HWND hwnd, UINT msg, WPARAM wParam,
                                        LPARAM lParam );

    void AdjustViewRect( RECT &rect );
};

#endif