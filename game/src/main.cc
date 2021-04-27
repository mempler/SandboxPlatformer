#include "Audio/AudioChannel.hh"

#include "Core/Engine.hh"

#include <glm/gtc/matrix_transform.hpp>

#include <iostream>

class SandboxGame : public BaseApp {
private:
    AudioChannel *m_pSoundEffectChannel;
    Audio *m_pAudio;

    glm::vec3 m_v3AudioPosition;

protected:
    void Init() override {
        m_pSoundEffectChannel = m_pEngine->GetAudioSystem().CreateChannel("audio://sound_effects");
        m_pAudio = m_pEngine->GetAudioSystem().LoadMonoAudio(m_pSoundEffectChannel, "file://audio.wav");

        m_pAudio->SetPitch(1.25f);
        m_pAudio->SetVolume(0.5f);
        m_pAudio->Play();
    }

    void Tick(float fDelta) override {
    }

    void Draw(float fDelta) override {
        //m_pEngine->GetBatcher().SubmitRectangle(NULL, glm::translate(glm::mat4(1.f), { 100.f, 100.f, 1.f }) * glm::scale(glm::mat4(1.f), { 100.f, 100.f, 1.f }));
    }
};

int main() {
    BaseApp *app = new SandboxGame;

    app->Run();

    return 0;
}
