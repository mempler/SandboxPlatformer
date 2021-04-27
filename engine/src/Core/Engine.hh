#pragma once

#include "pch.hh"

#include "Audio/AudioSystem.hh"

#include "Core/Managers/ShaderManager.hh"
#include "Core/Managers/TextureManager.hh"

#include "Graphics/VertexBatcher.hh"
#include "Graphics/Window.hh"

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

private:
};

class BaseApp {
public:
    BaseApp();
    ~BaseApp();

    void Run();

protected:
    virtual void Init() = 0;

    virtual void Tick() = 0;
    virtual void Draw() = 0;

protected:
    Engine *m_pEngine;
};
