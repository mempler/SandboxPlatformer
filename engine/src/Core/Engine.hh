#pragma once

#include "pch.hh"

#include "Core/Audio/AudioSystem.hh"
#include "Core/Debug/GameView.hh"
#include "Core/Debug/IResourceMonitor.hh"
#include "Core/Debug/Profiler.hh"
#include "Core/Graphics/Camera2D.hh"
#include "Core/Graphics/Surface/Surface.hh"
#include "Core/Graphics/VertexBatcher.hh"
#include "Core/Managers/FontManager.hh"
#include "Core/Managers/InputManager.hh"
#include "Core/Managers/ShaderManager.hh"
#include "Core/Managers/TextureManager.hh"

#include <stdint.h>

class Engine
{
  public:
    Engine( SurfaceDesc &surfaceDesc );
    ~Engine();

    Camera2D &GetCamera();
    BaseSurface *GetSurface();
    VertexBatcher &GetBatcher();
    TextureManager &GetTextureManager();
    ShaderManager &GetShaderManager();
    AudioSystem &GetAudioSystem();
    FontManager &GetFontManager();
    InputManager &GetInputManager();

    void AddView( bgfx::ViewId viID );
    void ResetTransform();

    void BeginFrame();
    void EndFrame();

    void Init();

  public:
    uint32_t GetResetFlags()
    {
        return m_uResetFlags;
    }

    void SetResetFlags( uint32_t uFlags )
    {
        m_uResetFlags = uFlags;
    }

  private:
    // Graphics
    void InitBGFX();
    BaseSurface *m_BaseSurface = 0;
    Camera2D m_Camera;
    VertexBatcher m_VertexBatcher;

    // Managers
    TextureManager m_TextureManager;
    ShaderManager m_ShaderManager;
    InputManager m_InputManager;
    FontManager m_FontManager;

    // Audio
    AudioSystem m_AudioSystem;

    // Debug Utils
#if PLATFORM_ANDROID
    bool m_bShowDebugUtils = true;
#else
    bool m_bShowDebugUtils = false;
#endif

    // Callback utils
    void OnResolutionChanged( BaseSurface *pSurface, uint32_t uWidth, uint32_t uHeight );

    GameView m_GameView;
    IResourceMonitor m_IResourceMonitor;
    Profiler m_Profiler;

    uint32_t m_uResetFlags = 0;

    bool m_bIsBGFXInitialized = false;

    int64_t m_iLastTime = 0;

    std::vector<bgfx::ViewId> m_vViews;

  private:
};

class BaseApp
{
  public:
    BaseApp();
    ~BaseApp();

    void Run();

  public:
    Engine *GetEngine() const
    {
        return m_pEngine;
    }

  protected:
    virtual void Init() = 0;

    virtual void Tick( float fDelta ) = 0;
    virtual void Draw( float fDelta ) = 0;

  protected:
    Engine *m_pEngine;
};

extern BaseApp *GetApp();
extern Engine *GetEngine();

#if PLATFORM_ANDROID
int SDL_main( int argc, char *argv [] );
    #define main SDL_main
#endif
