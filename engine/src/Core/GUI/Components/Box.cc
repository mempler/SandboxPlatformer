#include "Box.hh"

#include "Core/Engine.hh"
#include "Core/GUI/GUIComponent.hh"
#include "Core/Utils/Identifier.hh"

Box::Box() : GUIComponent() {
    OnDraw.connect<&Box::Draw>(this);
}

void Box::SetImage(const Identifier &identifier) {
    if (identifier != EmptyIdentifier) {
        m_pImage = GetEngine()->GetTextureManager().Load(identifier);
    } else {
        m_pImage = NULL;
    }
}

void Box::Draw(float fDelta) {
    GetEngine()->GetBatcher().SubmitRectangle(m_pImage, GetTransformation(), m_v4Color);
}
