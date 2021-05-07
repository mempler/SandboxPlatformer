#include "Core/Audio/AudioChannel.hh"
#include "Core/Engine.hh"
#include "Core/Graphics/Font/Label.hh"
#include "Core/Utils/Math.hh"

#include <glm/gtc/matrix_transform.hpp>

class SandboxGame : public BaseApp {
protected:
    void Init() override {
        m_pFont = m_pEngine->GetFontManager().LoadFromFile("file://Roboto-Regular.ttf", 512, 512, 64.f);
        m_pLabel.SetText({ 300, 300, 2 }, "TW // game engine", m_pFont);
    }

    void Tick(float fDelta) override {
    }

    void Draw(float fDelta) override {
        // TESTS
        glm::vec2 size = Label::CalculateTextSize("TW // game engine", m_pFont);

        m_pEngine->GetBatcher().SubmitRectangle(NULL, Math::CalcTransform({ 300, 300, 1 }, size), { 1, 0, 0, 1 });

        m_pLabel.Render();
    }

private:
    // AudioChannel *m_pSoundEffectChannel;
    // Audio *m_pAudio;

    glm::vec3 m_v3AudioPosition;
    Font *m_pFont;
    Label m_pLabel;
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
