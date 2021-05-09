#include "Game.hh"

#include "Core/Engine.hh"

// do not init world
Game::Game() : m_ItemInfoManager() {
}

void Game::Init() {
    // PRELOAD ITEMS, FOR NOW
    // Rock
    m_ItemInfoManager.Preload({ 0., 0, 32, 32 }, GetEngine()->GetTextureManager().Load("file://tiles1.png"));
    // Dirt
    m_ItemInfoManager.Preload({ 32, 0, 32, 32 }, GetEngine()->GetTextureManager().Load("file://tiles1.png"));
    // Grass
    m_ItemInfoManager.Preload({ 64, 0, 32, 32 }, GetEngine()->GetTextureManager().Load("file://tiles1.png"));

    m_World.Init(100, 60);

    for (uint16_t i = 0; i < 100; i++) m_World.PlaceFore(2, i, 0);

    for (uint16_t x = 0; x < 100; x++) {
        for (uint16_t y = 1; y < 60; y++) {
            m_World.PlaceFore(1, x, y);
        }
    }
}

void Game::Tick(float fDeltaTime) {
    m_World.Tick(fDeltaTime);
}

void Game::Draw() {
    m_World.Draw();
}

ItemInfoManager &Game ::GetItemInfoMan() {
    return m_ItemInfoManager;
}

World &Game::GetWorld() {
    return m_World;
}