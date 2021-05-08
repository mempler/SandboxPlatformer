#include "Core/Audio/AudioChannel.hh"
#include "Core/Engine.hh"
#include "Core/Graphics/Font/Label.hh"
#include "Core/Utils/Math.hh"

class SandboxGame : public BaseApp {
protected:
    void Init() override {
        m_pFont = m_pEngine->GetFontManager().LoadFromFile("file://Roboto-Regular.ttf", 256, 256, 22.f);
        m_lWatermark.SetText({ 0, 0, 999 }, "ICESDK DEMO", m_pFont);
        m_lWatermark.SetColor({ 1, 1, 1, .2 });

        glm::vec3 center = { m_pEngine->GetWindow().Width() / 2.f - m_lWatermark.GetSize().x / 2, m_pEngine->GetWindow().Height() - 100.f, 999.f };

        m_lWatermark.SetPosition(center);
    }

    void Tick(float fDelta) override {
    }

    void Draw(float fDelta) override {
        m_lWatermark.Render();
    }

private:
    Font *m_pFont;
    Label m_lWatermark;
};

static BaseApp *app = nullptr;

int main() {
    app = new SandboxGame;

    app->Run();

    return 0;
}

BaseApp *GetApp() {
    return app;
}
