#include "Engine.hh"

#include "Core/Audio/AudioSystem.hh"
#include "Core/Managers/ShaderManager.hh"
#include "Core/Managers/TextureManager.hh"
#include "Core/Utils/Timer.hh"

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