#pragma once

#include "pch.hh"

#include "Core/Audio/AudioSystem.hh"
#include "Core/Debug/GameView.hh"
#include "Core/Debug/IResourceMonitor.hh"
#include "Core/Debug/Profiler.hh"
#include "Core/Graphics/Camera2D.hh"
#include "Core/Graphics/VertexBatcher.hh"
#include "Core/Graphics/Window.hh"
#include "Core/Managers/ShaderManager.hh"
#include "Core/Managers/TextureManager.hh"

class Engine {
public:
    Engine();
    ~Engine();

    Camera2D &GetCamera();
    GameWindow &GetWindow();
    VertexBatcher &GetBatcher();
    TextureManager &GetTextureManager();
    ShaderManager &GetShaderManager();
    AudioSystem &GetAudioSystem();

    void BeginFrame();
    void EndFrame();

    void Init();

private:
    // Graphics
    GameWindow m_GameWindow;
    Camera2D m_Camera;
    VertexBatcher m_VertexBatcher;

    // Managers
    TextureManager m_TextureManager;
    ShaderManager m_ShaderManager;

    // Audio
    AudioSystem m_AudioSystem;

    // Debug Utils
    bool m_bShowDebugUtils = true;

    GameView m_GameView;
    IResourceMonitor m_IResourceMonitor;
    Profiler m_Profiler;

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