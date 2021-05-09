#pragma once

#include "pch.hh"

#include "Game/Managers/ItemInfoManager.hh"
#include "Game/World//World.hh"

class Game {
public:
    Game();
    ~Game(){};

    ItemInfoManager &GetItemInfoMan();

    World &GetWorld();

    void Init();
    void Tick(float fDeltaTime);
    void Draw();

private:
    // Item utilites
    ItemInfoManager m_ItemInfoManager;

    // Main handlers
    World m_World;
};

extern Game *GetGame();