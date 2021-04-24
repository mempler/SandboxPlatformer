#pragma once

#include "pch.hh"

#include "Graphics/Window.hh"

class Engine {
public:
private:
    GameWindow *m_pGameWindow = 0;
    // Audio
};

class BaseApp {
private:
    Engine *m_pEngine;

public:
    BaseApp();
    ~BaseApp();
};
