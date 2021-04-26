#pragma once

#include "pch.hh"

#include "Graphics/Window.hh"
#include "Graphics/VertexBatcher.hh"

class Engine {
public:
    Engine();
    ~Engine();

    GameWindow &GetWindow();
    VertexBatcher &GetBatcher();

    void BeginFrame();
    void EndFrame();

private:
    GameWindow m_GameWindow;
    VertexBatcher m_VertexBatcher;
    // Audio
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
