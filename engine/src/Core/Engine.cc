#include "Engine.hh"

//////////////////
//    Engine    //
//////////////////
Engine::Engine() : m_GameWindow() {
}

Engine::~Engine() {
}

void Engine::BeginFrame() {
    m_GameWindow.BeginFrame();
}

void Engine::EndFrame() {
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