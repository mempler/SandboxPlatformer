#include "Box.hh"

#include "Core/Engine.hh"
#include "Core/GUI/GUIComponent.hh"

#include "glm/packing.hpp"

#include <glm/gtx/color_encoding.hpp>

Box::Box() : GUIComponent() {
    OnDraw.connect<&Box::Draw>(this);
}

void Box::Draw(float fDelta) {
    GetEngine()->GetBatcher().SubmitRectangle(NULL, GetTransformation(), m_v4Color);
}
