#pragma once

#include "Core/GUI/GUIComponent.hh"
#include "Core/Graphics/Texture2D.hh"

class Box : public GUIComponent {
public:
    Box();

private:
    void Draw(float fDelta);
};
