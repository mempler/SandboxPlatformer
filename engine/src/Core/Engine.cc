#include "Engine.hh"

//////////////////
//    Engine    //
//////////////////
Engine::Engine() : m_GameWindow(), m_VertexBatcher() {
}

Engine::~Engine() {
}

VertexBatcher &Engine::GetBatcher() {
    return m_VertexBatcher;
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
}

void BaseApp::Run() {
    Init();

    while (true) {
        m_pEngine->BeginFrame();

        Tick();
        Draw();

        m_pEngine->EndFrame();
    }
}