#include "GUIComponent.hh"

void GUIComponent::Tick(float fDelta) {
    m_MoveTweener.Tick(fDelta);
    m_SizeTweener.Tick(fDelta);
    m_ScaleTweener.Tick(fDelta);
    m_RotateTweener.Tick(fDelta);
    m_ColorTweener.Tick(fDelta);

    if (m_MoveTweener.HasNext()) {
        m_v2Position = m_MoveTweener.Current();
        CalculateTransformation();
    }

    if (m_SizeTweener.HasNext()) {
        m_v2Size = m_SizeTweener.Current();
        CalculateTransformation();
    }

    if (m_ScaleTweener.HasNext()) {
        m_v2Scale = m_ScaleTweener.Current();
        CalculateTransformation();
    }

    if (m_RotateTweener.HasNext()) {
        m_fRotation = m_RotateTweener.Current();
        CalculateTransformation();
    }

    if (m_ColorTweener.HasNext()) {
        m_v4Color = m_ColorTweener.Current();
    }

    for (auto &child : m_pChildren) {
        child->OnTick(fDelta);
    }
}

void GUIComponent::Draw(float fDelta) {
    for (auto &child : m_pChildren) {
        child->OnDraw(fDelta);
    }
}
