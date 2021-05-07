#include "Core/Audio/AudioChannel.hh"
#include "Core/Engine.hh"
#include "Core/Graphics/Font/Label.hh"

#include <glm/gtc/matrix_transform.hpp>

class SandboxGame : public BaseApp {
protected:
    void Init() override {
        m_pFont = m_pEngine->GetFontManager().LoadFromFile("file://Roboto-Regular.ttf", 512, 512, 64.f);
        m_pLabel.SetText({ 0, 0, 2 }, "Trollface", m_pFont);
    }

    void Tick(float fDelta) override {
    }

    void Draw(float fDelta) override {
        // TESTS
        glm::vec2 size = Label::CalculateTextSize("Trollface", m_pFont);
        m_pEngine->GetBatcher().SubmitRectangle(NULL,
            glm::translate(glm::mat4(1.f), { m_pLabel.GetPosition().x, m_pLabel.GetPosition().y, 1 }) *
                glm::scale(glm::mat4(1.f), { m_pLabel.GetSize().x, m_pLabel.GetSize().y, 1.f }),
            { 1, 0, 0, 1 });

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
