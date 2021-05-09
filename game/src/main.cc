#include "Core/Audio/AudioChannel.hh"
#include "Core/Engine.hh"
#include "Core/Graphics/Font/Label.hh"
#include "Core/Utils/Math.hh"

#include "Game/Game.hh"

class Application : public BaseApp {
public:
    Game m_Game;

protected:
    void Init() override {
        m_pEngine->GetWindow().OnResize.connect<&Application::OnWindowResize>(this);

        m_pFont = m_pEngine->GetFontManager().LoadFromFile("file://Roboto-Regular.ttf", 256, 256, 22.f);
        m_lWatermark.SetText({ 0, 0, 999.f }, "ICESDK DEMO", m_pFont);
        m_lWatermark.SetColor({ 1, 1, 1, .2 });

        glm::vec3 center = { m_pEngine->GetWindow().Width() / 2.f - m_lWatermark.GetSize().x / 2, m_pEngine->GetWindow().Height() - 100.f, 999.f };

        m_lWatermark.SetPosition(center);

        m_Game.Init();
    }

    void Tick(float fDelta) override {
        m_Game.Tick(fDelta);
    }

    void Draw(float fDelta) override {
        m_Game.Draw();

        m_lWatermark.Render();
    }

private:
    void OnWindowResize(GameWindow *pGameWindow, uint32_t iWidth, uint32_t iHeight) {
        glm::vec3 center = { m_pEngine->GetWindow().Width() / 2.f - m_lWatermark.GetSize().x / 2.f, m_pEngine->GetWindow().Height() - 100.f, 999.f };
        m_lWatermark.SetPosition(center);
    }

private:
    Font *m_pFont;
    Label m_lWatermark;
};

static Application *app = nullptr;

int main() {
    app = new Application;

    app->Run();

    return 0;
}

BaseApp *GetApp() {
    return app;
}

Game *GetGame() {
    return &app->m_Game;
}