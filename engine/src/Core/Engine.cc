#include "pch.hh"

#include "Engine.hh"

#include "Core/Audio/AudioSystem.hh"
#include "Core/Debug/DefaultLayout.hh"
#include "Core/Debug/IResourceMonitor.hh"
#include "Core/Managers/InputManager.hh"
#include "Core/Managers/ShaderManager.hh"
#include "Core/Managers/TextureManager.hh"
#include "Core/Utils/Timer.hh"

#include "bgfx/bgfx.h"

#include <SDL_keycode.h>
#include <imgui.h>

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
Engine::Engine() 
  : m_GameWindow(), 
    m_Camera({ 0.f, 0.f }, { (float)m_GameWindow.Width(), (float)m_GameWindow.Height() }),
    m_VertexBatcher()
{
        ZoneScoped;
    m_Camera.SetUniformTransform(0);
}
// clang-format on

Engine::~Engine()
{
    ZoneScoped;
}

GameWindow &Engine::GetWindow()
{
    return m_GameWindow;
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

void Engine::Init()
{
    ZoneScoped;

    Console::Init();

    m_ShaderManager.LoadDefaultShaders();
    m_AudioSystem.Init();
    m_Camera.Init();
    m_InputManager.Init();

    m_VertexBatcher.Init( m_TextureManager );

#if ENGINE_DEBUG
    RegisterDebugUtil<IResourceMonitor>( true );
    RegisterDebugUtil<GameView>( true );
    RegisterDebugUtil<Profiler>( true );
#endif
}

void Engine::BeginFrame()
{
    FrameMarkStart( "Engine Frame" );

    ZoneScoped;
    m_GameWindow.BeginFrame();
    m_VertexBatcher.BeginFrame();

#if ENGINE_DEBUG
    if ( ImGui::IsKeyPressed( SDL_SCANCODE_F8, false ) )
    {
        m_bShowDebugUtils = !m_bShowDebugUtils;

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
            for ( auto &[ tab, dbgUtil ] : m_vDebugUtils )
            {
                if ( ImGui::BeginMenu( tab ) )
                {
                    if ( ImGui::MenuItem( dbgUtil->Name() ) )
                    {  // Toggle
                        dbgUtil->SetShowing( !dbgUtil->IsShowing() );
                    }

                    ImGui::EndMenu();
                }
            }

            ImGui::EndMenuBar();
        }

        for ( auto &[ tab, dbgUtil ] : m_vDebugUtils )
        {
            if ( dbgUtil->IsShowing() )
            {
                dbgUtil->Draw();
            }
        }

        ImGui::End();
    }
#endif
}

void Engine::EndFrame()
{
    ZoneScoped;
    m_VertexBatcher.EndFrame();
    m_GameWindow.EndFrame();

    FrameMarkEnd( "Engine Frame" );
}

//////////////////
//    BaseApp   //
//////////////////
BaseApp::BaseApp() : m_pEngine( new Engine )
{
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
    while ( !m_pEngine->GetWindow().ShouldExit() )
    {
        auto elapsed = timer.elapsed();
        timer.reset();

        m_pEngine->BeginFrame();

        Tick( elapsed );
        Draw( elapsed );

        m_pEngine->EndFrame();
    }
}
