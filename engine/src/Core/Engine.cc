#include "pch.hh"

#include "Engine.hh"

#include "Core/Audio/AudioSystem.hh"
#include "Core/Debug/DefaultLayout.hh"
#include "Core/Graphics/Surface/PlatformSurface.hh"
#include "Core/Graphics/Surface/Surface.hh"
#include "Core/Managers/InputManager.hh"
#include "Core/Managers/ShaderManager.hh"
#include "Core/Managers/TextureManager.hh"
#include "Core/Utils/Timer.hh"

#include "bgfx/bgfx.h"

#include <SDL_keycode.h>
#include <imgui.h>
#include <stdint.h>

//////////////////
//    Engine    //
//////////////////
Engine *GetEngine()
{
    ZoneScoped;
    return GetApp()->GetEngine();
}

// Turn off formatting, there is some weird shit going on
// clang-format off
Engine::Engine(SurfaceDesc &surfaceDesc) 
  : m_BaseSurface(new PlatformSurface(surfaceDesc)), 
    m_Camera({ 0.f, 0.f }, 
                { (float)m_BaseSurface->GetWidth(), (float)m_BaseSurface->GetHeight() }),
    m_VertexBatcher(), 
    m_IResourceMonitor(this), 
    m_GameView(this),
    m_Profiler(this)
{
    ZoneScoped;
}
// clang-format on

Engine::~Engine()
{
    ZoneScoped;
}

BaseSurface *Engine::GetSurface()
{
    return m_BaseSurface;
}

Camera2D &Engine::GetCamera()
{
    return m_Camera;
}

VertexBatcher &Engine::GetBatcher()
{
    return m_VertexBatcher;
}

TextureManager &Engine::GetTextureManager()
{
    return m_TextureManager;
}

ShaderManager &Engine::GetShaderManager()
{
    return m_ShaderManager;
}

AudioSystem &Engine::GetAudioSystem()
{
    return m_AudioSystem;
}

FontManager &Engine::GetFontManager()
{
    return m_FontManager;
}

InputManager &Engine::GetInputManager()
{
    return m_InputManager;
}

void Engine::AddView( bgfx::ViewId viID )
{
    ZoneScoped;
    bgfx::setViewClear( viID, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x0000000FF, 1.0f, 0 );
    bgfx::setViewRect( viID, 0, 0, bgfx::BackbufferRatio::Equal );

    m_vViews.push_back( viID );
}

void Engine::ResetTransform()
{
    ZoneScoped;
    for ( auto &i : m_vViews )
    {
        bgfx::setViewRect( i, 0, 0, bgfx::BackbufferRatio::Equal );
        GetEngine()->GetCamera().SetUniformTransform( i );
    }
}

void Engine::Init()
{
    ZoneScoped;

    InitBGFX();

    AddView( 0 );
    m_Camera.SetUniformTransform( 0 );

    m_BaseSurface->OnResolutionChanged.connect<&Engine::OnResolutionChanged>( this );

    m_ShaderManager.LoadDefaultShaders();
    m_AudioSystem.Init();
    m_Camera.Init();
    m_InputManager.Init();

    m_VertexBatcher.Init( m_TextureManager );

    // Show by default
    m_IResourceMonitor.SetShowing( true );
    m_GameView.SetShowing( true );
    m_Profiler.SetShowing( false );
}

void Engine::InitBGFX()
{
    ZoneScoped;

    bgfx::PlatformData platformData;

#if PLATFORM_WIN32
    platformData.nwh = m_BaseSurface->GetHandle();
#elif PLATFORM_ANDROID
    platformData.ndt = 0;
    platformData.nwh = (void *) wmInfo.info.android.window;

    // Delete existing surface
    // TODO: use EGL for Wayland on Linux
    eglDestroySurface( eglGetDisplay( EGL_DEFAULT_DISPLAY ),
                       wmInfo.info.android.surface );
#elif PLATFORM_EMSCRIPTEN
    platformData.nwh = (void *) "#canvas";
#elif PLATFORM_LINUX
    platformData.ndt = wmInfo.info.x11.display;
    platformData.nwh = (void *) (uintptr_t) wmInfo.info.x11.window;
#else
    #error "Platform not implemented!"
#endif

    // Initialize BGFX using our Platform data
    bgfx::Init bgfxInit;
    // bgfxInit.debug = true; // This slows down the renderer quite a bit

    bgfxInit.type = bgfx::RendererType::Count;  // Automatically choose a renderer. OpenGL
                                                // for RENDERDOC.
    bgfxInit.platformData = platformData;
    bgfxInit.resolution.width = GetSurface()->GetWidth();
    bgfxInit.resolution.height = GetSurface()->GetHeight();
    bgfxInit.resolution.reset = BGFX_RESET_VSYNC;
    bgfxInit.limits.transientVbSize = 134217728;

    Console::Info( "Initializing BGFX..." );
    if ( !bgfx::init( bgfxInit ) )
    {
        Console::Fatal( "Failed to initialize BGFX!" );
        return;
    }

    Console::Info( "Game Window Initialized..." );
    Console::Info( "BGFX Initialized..." );

#if PLATFORM_ANDROID
    // Set display size
    // SDL_DisplayMode displayMode;
    // SDL_GetDesktopDisplayMode( 0, &displayMode );

    // m_iWidth = static_cast<uint32_t>( displayMode.w );
    // m_iHeight = static_cast<uint32_t>( displayMode.h );

    bgfx::reset( m_iWidth, m_iHeight, BGFX_RESET_VSYNC | BGFX_RESET_FULLSCREEN );
    bgfx::setViewRect( 0, 0, 0, bgfx::BackbufferRatio::Equal );
#endif

#if ENGINE_DEBUG
    // ImGui::CreateContext();
    // ImPlot::CreateContext();

    // ImGuiIO &io = ImGui::GetIO();
    // (void) io;
    // io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls
    // io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;   // Enable Gamepad Controls
    // io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;      // Enable Docking

    // #if PLATFORM_ANDROID
    // ImGui::GetCurrentContext()->CurrentDpiScale = 5;

    // io.ConfigFlags |= ImGuiConfigFlags_IsTouchScreen;
    // #endif

    // ImGui::StyleColorsDark();

    // ImGui_Implbgfx_Init( 999 );
    // #if PLATFORM_WIN32
    // imgui_impl( m_SDLWindow );
    // #elif PLATFORM_LINUX
    // ImGui_ImplSDL2_InitForOpenGL( m_SDLWindow, nullptr );
    // #endif
#endif
}

void Engine::BeginFrame()
{
    FrameMarkStart( "Engine Frame" );

    ZoneScoped;

    m_BaseSurface->Poll();

    const auto now = bx::getHPCounter();
    const auto frameTime = now - m_iLastTime;
    m_iLastTime = now;

    const auto freq = static_cast<float>( bx::getHPFrequency() );
    const auto delta = static_cast<float>( frameTime ) / freq;
    bgfx::touch( 0 );

    m_VertexBatcher.BeginFrame();

#if ENGINE_DEBUG
    if ( ImGui::IsKeyPressed( SDL_SCANCODE_F8, false ) )
    {
        m_bShowDebugUtils = !m_bShowDebugUtils;

        if ( !m_bShowDebugUtils && m_GameView.IsShowing() )
            bgfx::setViewFrameBuffer( 0, BGFX_INVALID_HANDLE );  // Reset framebuffer
    }

    if ( m_bShowDebugUtils )
    {
        ImGuiWindowFlags window_flags =
            ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
        window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse
                        | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
        window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus
                        | ImGuiWindowFlags_NoNavFocus | ImGuiWindowFlags_NoScrollbar
                        | ImGuiWindowFlags_NoScrollWithMouse;

        const ImGuiViewport *viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos( viewport->WorkPos );
        ImGui::SetNextWindowSize( viewport->WorkSize );
        ImGui::SetNextWindowViewport( viewport->ID );

        ImGui::Begin( "EngineDebug", nullptr, window_flags );

        ImGuiID dockspace_id = ImGui::GetID( "EngineDockSpace" );
        if ( ImGui::DockBuilderGetNode( dockspace_id ) == NULL )
            DefaultImGuiLayout( this, dockspace_id );

        ImGui::DockSpace( dockspace_id, ImVec2( 0, 0 ) );

        if ( ImGui::BeginMenuBar() )
        {
            if ( ImGui::BeginMenu( "Engine" ) )
            {
                if ( ImGui::MenuItem( "IResource Monitor" ) )
                {  // Toggle
                    m_IResourceMonitor.SetShowing( !m_IResourceMonitor.IsShowing() );
                }

                if ( ImGui::MenuItem( "Game View" ) )
                {  // Toggle
                    m_GameView.SetShowing( !m_GameView.IsShowing() );

                    m_GameView.Draw();  // Draw one more time
                }

                if ( ImGui::MenuItem( "Profiler" ) )
                {  // Toggle
                    m_Profiler.SetShowing( !m_Profiler.IsShowing() );
                }

                ImGui::EndMenu();
            }
            ImGui::EndMenuBar();
        }

        if ( m_IResourceMonitor.IsShowing() )
        {
            m_IResourceMonitor.Draw();
        }

        if ( m_GameView.IsShowing() )
        {
            m_GameView.Draw();
        }

        if ( m_Profiler.IsShowing() )
        {
            m_Profiler.Draw();
        }

        ImGui::End();
    }
#endif
}

void Engine::EndFrame()
{
    ZoneScoped;
    m_VertexBatcher.EndFrame();

    bgfx::frame();

    FrameMarkEnd( "Engine Frame" );
}

void Engine::OnResolutionChanged( BaseSurface *pSurface, uint32_t uWidth,
                                  uint32_t uHeight )
{
    
}

//////////////////
//    BaseApp   //
//////////////////
BaseApp::BaseApp()
{
    ZoneScoped;
    Console::Init();
    Console::Info( "Initializing Ice..." );

    SurfaceDesc desc;
    desc.sTitle = "Sandbox Platformer: Game Surface";
    desc.ivRes = { 1280, 720 };
    desc.eFlags |= eWindowFlags::Centered | eWindowFlags::Resizable;
    m_pEngine = new Engine( desc );
}

BaseApp::~BaseApp()
{
    delete m_pEngine;
}

void BaseApp::Run()
{
    {
        ZoneScoped;
        m_pEngine->Init();

        Init();
    }

    Timer timer;
    while ( !m_pEngine->GetSurface()->ShouldExit() )
    {
        auto elapsed = timer.elapsed();
        timer.reset();

        m_pEngine->BeginFrame();

        Tick( elapsed );
        Draw( elapsed );

        m_pEngine->EndFrame();
    }
}
