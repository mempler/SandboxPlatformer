#include "Engine.hh"

#include "Core/Audio/AudioSystem.hh"
#include "Core/Managers/ShaderManager.hh"
#include "Core/Managers/TextureManager.hh"
#include "Core/Utils/Timer.hh"

#include <imgui.h>
#include <imgui_impl_sdl.h>

//////////////////
//    Engine    //
//////////////////
Engine *GetEngine() {
    return GetApp()->GetEngine();
}

Engine::Engine() : m_GameWindow(), m_VertexBatcher() {
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
}

void Engine::BeginFrame() {
    m_GameWindow.BeginFrame();
    m_VertexBatcher.BeginFrame();

    if (ImGui::BeginMainMenuBar()) {
        if (ImGui::BeginMenu("Engine")) {
            if (ImGui::MenuItem("Resource Monitor")) {
            }
            if (ImGui::MenuItem("Texture Manager Monitor")) {
            }
            if (ImGui::MenuItem("Shader Manager Monitor")) {
            }
            if (ImGui::MenuItem("Audio Engine Monitor")) {
            }

            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }
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