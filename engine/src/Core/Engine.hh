#pragma once

#include "pch.hh"

#include "Graphics/Window.hh"

class Engine {
public:
    Engine();
    ~Engine();

    GameWindow &GetWindow();

    void BeginFrame();
    void EndFrame();

private:
    GameWindow m_GameWindow;
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

private:
    Engine *m_pEngine;
};
