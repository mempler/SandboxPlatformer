#include "Core/Audio/AudioChannel.hh"
#include "Core/Engine.hh"

#include <glm/gtc/matrix_transform.hpp>

#include <freetype-gl.h>

#include <iostream>

class SandboxGame : public BaseApp {
protected:
    void Init() override {
        m_pFont = m_pEngine->GetFontManager().LoadFromFile("file://arial.ttf", 128, 128, 64.f);

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

        // draw normal 100x100 rect
        glm::mat4 trans = glm::translate(glm::mat4(1.f), { 20, 20, 1.f }) * glm::scale(glm::mat4(1.f), { 128.f, 128.f, 1.f });
        m_pEngine->GetBatcher().SubmitRectangle(0, trans);
    }

private:
    // AudioChannel *m_pSoundEffectChannel;
    // Audio *m_pAudio;

    glm::vec3 m_v3AudioPosition;
    Font *m_pFont;
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
