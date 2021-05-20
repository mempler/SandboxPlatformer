#pragma once

#include <cstdint>

#include "Core/Graphics/Surface/Surface.hh"

#include <Platform.hh>

#if !PLATFORM_ANDROID
    #if PLATFORM_LINUX

class X11Surface : public BaseSurface
{
  public:
    X11Surface( SurfaceDesc &desc );
    ~X11Surface();

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

    bgfx::PlatformData GetPlatformData() override;

  private:
    uintptr_t m_hDisplay;
    uintptr_t m_hWindow;
    uintptr_t m_hScreen;
};

    #endif
#endif