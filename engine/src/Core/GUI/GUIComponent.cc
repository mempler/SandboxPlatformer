#include "GUIComponent.hh"

void GUIComponent::Tick(float fDelta) {
    for (auto &child : m_pChildren) {
        child->OnTick(fDelta);
    }
}

void GUIComponent::Draw(float fDelta) {
    for (auto &child : m_pChildren) {
        child->OnDraw(fDelta);
    }
}
