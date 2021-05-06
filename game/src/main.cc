#include "Core/Audio/AudioChannel.hh"
#include "Core/Engine.hh"
#include "Core/Graphics/Font/Label.hh"

#include <glm/gtc/matrix_transform.hpp>

class SandboxGame : public BaseApp {
protected:
    void Init() override {
        m_pFont = m_pEngine->GetFontManager().LoadFromFile("file://Roboto-Regular.ttf", 512, 512, 64.f);
        m_pLabel.SetText({ 100, 100, 1 }, "YEEEEEEEA BOIIIIIIIIIIIIIIIIIII", m_pFont);

        // m_pSoundEffectChannel = m_pEngine->GetAudioSystem().CreateChannel("audio://sound_effects");
        // m_pAudio = m_pEngine->GetAudioSystem().LoadMonoAudio(m_pSoundEffectChannel, "file://audio.wav");

        // m_pAudio->SetPitch(1.25f);
        // m_pAudio->SetVolume(0.5f);
        // m_pAudio->Play();
    }

    void Tick(float fDelta) override {
    }

    void Draw(float fDelta) override {
        // TESTS
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
