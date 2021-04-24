#include "Core/Engine.hh"

#include <iostream>

class SandboxGame : public BaseApp
{
};

int main()
{
    BaseApp* app = new SandboxGame;

    std::cout << "Hello World" << std::endl;

    return 0;
}
