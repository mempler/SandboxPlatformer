#include "Audio/AudioChannel.hh"

#include "Core/Engine.hh"

#include <glm/gtc/matrix_transform.hpp>

#include <iostream>

class SandboxGame : public BaseApp {
protected:
    void Init() override {
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
