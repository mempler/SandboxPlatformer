#include "Game.hh"

#include "Core/Engine.hh"
#include "Core/Graphics/Texture2D.hh"

#include "Game/Player/Player.hh"

void Game::OnGameResize(GameWindow *pGameWindow, uint32_t iWidth, uint32_t iHeight) {
    if (m_World.IsValid()) {
        if (bgfx::isValid(m_World.GetFrameBuffer()))
            bgfx::destroy(m_World.GetFrameBuffer());

        m_World.GetFrameBuffer() = bgfx::createFrameBuffer(pGameWindow->Width(), pGameWindow->Height(), bgfx::TextureFormat::RGBA8, g_uFrameBufferFlags);
    }
}

Game::Game() : m_ItemInfoManager(), m_World(), m_Player() {
}

void Game::Init() {
    // PREINIT EVENTS
    GetEngine()->GetWindow().OnResize.connect<&Game::OnGameResize>(this);
    GetEngine()->GetInputManager().OnKeyDown.connect<&Player::OnKeyDown>(&m_Player);
    GetEngine()->GetInputManager().OnKeyDown.connect<&Player::OnKeyRelease>(&m_Player);

    // PREINIT VIEWS
    GetEngine()->GetWindow().AddView(2); // World tile layer
    GetEngine()->GetWindow().ResetTransform();

    // PRELOAD ITEMS, FOR NOW
    // Rock
    m_ItemInfoManager.Preload({ 0., 0, 32, 32 }, GetEngine()->GetTextureManager().CreateTextureFromFile("file://tiles1.png", TEXTURE_FORMAT_NEAREST));
    // Dirt
    m_ItemInfoManager.Preload({ 32, 0, 32, 32 }, GetEngine()->GetTextureManager().CreateTextureFromFile("file://tiles1.png", TEXTURE_FORMAT_NEAREST));
    // Grass
    m_ItemInfoManager.Preload({ 64, 0, 32, 32 }, GetEngine()->GetTextureManager().CreateTextureFromFile("file://tiles1.png", TEXTURE_FORMAT_NEAREST));

    m_World.Init(100, 60);

    for (uint16_t i = 0; i < 20; i++) m_World.PlaceFore(2, i, 0);

    for (uint16_t x = 0; x < 20; x++) {
        for (uint16_t y = 1; y < 10; y++) {
            m_World.PlaceFore(1, x, y);
        }
    }

    m_World.OnPlayerEnter();
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

Player &Game::GetLocalPlayer() {
    return m_Player;
}