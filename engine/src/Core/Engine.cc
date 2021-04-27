#include "Engine.hh"

#include "Core/Managers/ShaderManager.hh"
#include "Core/Managers/TextureManager.hh"

//////////////////
//    Engine    //
//////////////////
Engine::Engine() : m_GameWindow(), m_VertexBatcher() {
}

Engine::~Engine() {
    m_TextureManager.DestroyAll();
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

void Engine::BeginFrame() {
    m_GameWindow.BeginFrame();
    m_VertexBatcher.BeginFrame();
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
    Init();

    while (!m_pEngine->GetWindow().ShouldExit()) {
        m_pEngine->BeginFrame();

        Tick();
        Draw();

        m_pEngine->EndFrame();
    }
}