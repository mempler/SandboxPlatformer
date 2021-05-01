#include "Engine.hh"

#include "Core/Audio/AudioSystem.hh"
#include "Core/Debug/DefaultLayout.hh"
#include "Core/Managers/ShaderManager.hh"
#include "Core/Managers/TextureManager.hh"
#include "Core/Utils/Timer.hh"

#include "bgfx/bgfx.h"

#include <SDL_keycode.h>
#include <imgui.h>

// EASTL Allocators
void *operator new[](size_t size, const char *name, int flags, unsigned debugFlags, const char *file, int line) {
    return new uint8_t[size];
}

void *operator new[](size_t size, size_t alignment, size_t offset, const char *name, int flags, unsigned debugFlags, const char *file, int line) {
    // Joinked from their allocator

    size_t adjustedAlignment = (alignment > EA_PLATFORM_PTR_SIZE) ? alignment : EA_PLATFORM_PTR_SIZE;

    void *p = new char[size + adjustedAlignment + EA_PLATFORM_PTR_SIZE];
    void *pPlusPointerSize = (void *)((uintptr_t)p + EA_PLATFORM_PTR_SIZE);
    void *pAligned = (void *)(((uintptr_t)pPlusPointerSize + adjustedAlignment - 1) & ~(adjustedAlignment - 1));

    void **pStoredPtr = (void **)pAligned - 1;
    EASTL_ASSERT(pStoredPtr >= p);
    *(pStoredPtr) = p;

    EASTL_ASSERT(((size_t)pAligned & ~(alignment - 1)) == (size_t)pAligned);

    return pAligned;
}

//////////////////
//    Engine    //
//////////////////
Engine *GetEngine() {
    return GetApp()->GetEngine();
}

Engine::Engine() : m_GameWindow(), m_VertexBatcher(), m_IResourceMonitor(this), m_GameView(this), m_Profiler(this) {
}

Engine::~Engine() {
}

GameWindow &Engine::GetWindow() {
    return m_GameWindow;
}

VertexBatcher &Engine::GetBatcher() {
    return m_VertexBatcher;
}

TextureManager &Engine::GetTextureManager() {
    return m_TextureManager;
}

ShaderManager &Engine::GetShaderManager() {
    return m_ShaderManager;
}

AudioSystem &Engine::GetAudioSystem() {
    return m_AudioSystem;
}

void Engine::Init() {
    m_ShaderManager.LoadDefaultShaders();
    m_AudioSystem.Init();

    m_VertexBatcher.Init(m_TextureManager);

    // Show by default
    m_IResourceMonitor.SetShowing(true);
    m_GameView.SetShowing(true);
    m_Profiler.SetShowing(true);
}

void Engine::BeginFrame() {
    m_GameWindow.BeginFrame();
    m_VertexBatcher.BeginFrame();

#if ENGINE_DEBUG
    if (ImGui::IsKeyPressed(SDL_SCANCODE_F8, false)) {
        m_bShowDebugUtils = !m_bShowDebugUtils;

        if (!m_bShowDebugUtils && m_GameView.IsShowing())
            bgfx::setViewFrameBuffer(0, BGFX_INVALID_HANDLE); // Reset framebuffer
    }

    if (m_bShowDebugUtils) {
        ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
        window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
        window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse;

        const ImGuiViewport *viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->WorkPos);
        ImGui::SetNextWindowSize(viewport->WorkSize);
        ImGui::SetNextWindowViewport(viewport->ID);

        ImGui::Begin("EngineDebug", nullptr, window_flags);

        ImGuiID dockspace_id = ImGui::GetID("EngineDockSpace");
        if (ImGui::DockBuilderGetNode(dockspace_id) == NULL)
            DefaultImGuiLayout(this, dockspace_id);

        ImGui::DockSpace(dockspace_id, ImVec2(0, 0));

        if (ImGui::BeginMenuBar()) {
            if (ImGui::BeginMenu("Engine")) {
                if (ImGui::MenuItem("IResource Monitor")) { // Toggle
                    m_IResourceMonitor.SetShowing(!m_IResourceMonitor.IsShowing());
                }

                if (ImGui::MenuItem("Game View")) { // Toggle
                    m_GameView.SetShowing(!m_GameView.IsShowing());

                    m_GameView.Draw(); // Draw one more time
                }

                if (ImGui::MenuItem("Profiler")) { // Toggle
                    m_Profiler.SetShowing(!m_Profiler.IsShowing());
                }

                ImGui::EndMenu();
            }
            ImGui::EndMenuBar();
        }

        if (m_IResourceMonitor.IsShowing()) {
            m_IResourceMonitor.Draw();
        }

        if (m_GameView.IsShowing()) {
            m_GameView.Draw();
        }

        if (m_Profiler.IsShowing()) {
            m_Profiler.Draw();
        }

        ImGui::End();
    }
#endif
}

void Engine::EndFrame() {
    m_VertexBatcher.EndFrame();
    m_GameWindow.EndFrame();
}

//////////////////
//    BaseApp   //
//////////////////
BaseApp::BaseApp() : m_pEngine(new Engine) {
}

BaseApp::~BaseApp() {
    delete m_pEngine;
}

void BaseApp::Run() {
    m_pEngine->Init();

    Init();

    Timer timer;
    while (!m_pEngine->GetWindow().ShouldExit()) {
        auto elapsed = timer.elapsed();
        timer.reset();

        m_pEngine->BeginFrame();

        Tick(elapsed);
        Draw(elapsed);

        m_pEngine->EndFrame();
    }
}