#include "Core/Engine.hh"
#include "Core/GUI/Components/Box.hh"
#include "Core/GUI/GUI.hh"

class SandboxGame : public BaseApp {
protected:
    void Init() override {
        Box *box = GetEngine()->GetGUI()->Add<Box>("gui://random_box");
        box->m_v2Position = { 100.0f, 100.0f };
        box->m_v4Color = { 1.0f, 0.0f, 0.0f, 1.0f };
        box->m_v2Size = { 500.f, 500.f };
        box->m_eOrigin = Origin::Center;

        box->CalculateTransformation();

        //box->RotateTo(360)->Easing(EasingType::circularInOut)->Within(1000)->Repeat()->Easing(EasingType::backInOut)->Repeat();
        //box->ScaleTo({ 1.5, 1.5 })->Easing(EasingType::Linear)->Within(1000)->Repeat()->Repeat();

        box->FadeTo({ 0.5, 0.5, 1.0, 1.0 })->Easing(EasingType::elasticOut)->Within(1000);
        box->SetImage("file://yes.png");
    }

    void Tick(float fDelta) override {
    }

    void Draw(float fDelta) override {
        // TESTS
    }

private:
};

BaseApp *GetApp() {
    return app;
}
