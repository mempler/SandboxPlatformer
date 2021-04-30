#pragma once

#include "pch.hh"

#include "Core/Audio/AudioSystem.hh"
#include "Core/Debug/IResourceMonitor.hh"
#include "Core/Graphics/VertexBatcher.hh"
#include "Core/Graphics/Window.hh"
#include "Core/Managers/ShaderManager.hh"
#include "Core/Managers/TextureManager.hh"

class Engine {
public:
    Engine();
    ~Engine();

    GameWindow &GetWindow();
    VertexBatcher &GetBatcher();
    TextureManager &GetTextureManager();
    ShaderManager &GetShaderManager();
    AudioSystem &GetAudioSystem();

    void BeginFrame();
    void EndFrame();

    void Init();

private:
    GameWindow m_GameWindow;
    VertexBatcher m_VertexBatcher;

    // Managers
    TextureManager m_TextureManager;
    ShaderManager m_ShaderManager;

    // Audio
    AudioSystem m_AudioSystem;

    // Debug Utils
    IResourceMonitor m_IResourceMonitor;

private:
};

class BaseApp {
public:
    BaseApp();
    ~BaseApp();

    void Run();

public:
    Engine *GetEngine() const {
        return m_pEngine;
    }

protected:
    virtual void Init() = 0;

    virtual void Tick(float fDelta) = 0;
    virtual void Draw(float fDelta) = 0;

protected:
    Engine *m_pEngine;
};

extern BaseApp *GetApp();
extern Engine *GetEngine();