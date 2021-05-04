#include "Core/Audio/AudioChannel.hh"
#include "Core/Engine.hh"
#include "Core/GUI/Components/Box.hh"
#include "Core/GUI/GUI.hh"

#include "glm/ext/quaternion_common.hpp"
#include "glm/gtx/easing.hpp"

#include <Core/Utils/Tweener.hh>

#include <iostream>

class SandboxGame : public BaseApp {
protected:
    Tweener<glm::vec2> tweener;

    void Init() override {
        Box *box = GetEngine()->GetGUI()->Add<Box>("gui://random_box");
        box->m_v2Position = { 100.0f, 100.0f };
        box->m_v4Color = { 1.0f, 0.0f, 0.0f, 1.0f };
        box->m_v2Size = { 100.f, 100.f };

        box->CalculateTransformation();

        box->ScaleTo({ 1.5, 1.5 })->Easing(EasingType::elasticOut)->Within(1000);
        box->FadeTo({ 0.0, 0.0, 1.0, 1.0 })->Easing(EasingType::elasticOut)->Within(1000);
    }

    void Tick(float fDelta) override {
    }

    void Draw(float fDelta) override {
    }

private:
    glm::vec3 m_v3AudioPosition;
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
