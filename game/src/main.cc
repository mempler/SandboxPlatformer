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
        box->m_v4Color = { 1.0f, 0.0f, 0.0f, 1.0f };
        box->m_v2Size = { 100.f, 100.f };

        box->CalculateTransformation();

        tweener.From(CurrentValue)->To(TargetValue)->Easing(EasingType::Linear)->Within(1000)->Repeat(3);
    }

    glm::vec2 TargetValue = { -250, 250 };
    glm::vec2 CurrentValue = { 0, 0 };

    void Tick(float fDelta) override {
        tweener.Tick(fDelta);

        glm::vec2 eased = tweener.Current();
        std::cout << "Value: X" << eased.x << " Y" << eased.y << std::endl;
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
