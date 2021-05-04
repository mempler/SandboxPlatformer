#pragma once

#include "Core/GUI/GUIComponent.hh"
#include "Core/Graphics/Texture2D.hh"
#include "Core/Utils/Identifier.hh"

class Box : public GUIComponent {
public:
    Box();

    void SetImage(Identifier const &identifier = EmptyIdentifier);

private:
    Texture2D *m_pImage = nullptr;

    void Draw(float fDelta);
};
