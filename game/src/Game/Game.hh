#pragma once

#include "Core/Graphics/Window.hh"

#include "Game/Managers/ItemInfoManager.hh"
#include "Game/World/World.hh"

// not the best place to put
constexpr uintptr_t g_uFrameBufferFlags =
    BGFX_TEXTURE_RT | BGFX_SAMPLER_MIN_POINT | BGFX_SAMPLER_MAG_POINT | BGFX_SAMPLER_MIP_POINT | BGFX_SAMPLER_U_CLAMP | BGFX_SAMPLER_V_CLAMP;

class Game {
public:
    Game();
    ~Game(){};

    ItemInfoManager &GetItemInfoMan();

    World &GetWorld();

    void Init();
    void Tick(float fDeltaTime);
    void Draw();

    void OnGameResize(GameWindow *pGameWindow, uint32_t iWidth, uint32_t iHeight);

private:
    // Item utilites
    ItemInfoManager m_ItemInfoManager;

    // Main handlers
    World m_World;
};

extern Game *GetGame();