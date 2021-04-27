#include "Audio/AudioChannel.hh"

#include "Core/Engine.hh"

#include <glm/gtc/matrix_transform.hpp>

#include <iostream>

class SandboxGame : public BaseApp {
private:
    AudioChannel *m_pSoundEffectChannel;

protected:
    void Init() override {
        m_pSoundEffectChannel = m_pEngine->GetAudioSystem().CreateChannel("audio://sound_effects");
    }

    void Tick() override {
    }

    void Draw() override {
        m_pEngine->GetBatcher().SubmitRectangle(NULL, glm::translate(glm::mat4(1.f), { 100.f, 100.f, 1.f }) * glm::scale(glm::mat4(1.f), { 100.f, 100.f, 1.f }));
    }
};

int main() {
    BaseApp *app = new SandboxGame;

    app->Run();

    return 0;
}
