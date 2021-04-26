#include "Core/Engine.hh"

#include <iostream>

class SandboxGame : public BaseApp {
protected:
    void Init() override {
    }

    void Tick() override {
    }
    void Draw() override {
    }
};

int main() {
    BaseApp *app = new SandboxGame;

    app->Run();

    return 0;
}
