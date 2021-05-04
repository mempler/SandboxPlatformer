#pragma once

#include "Core/Managers/InputManager.hh"
#include "Core/Utils/Identifier.hh"
#include "Core/Utils/Tweener.hh"

#include "glm/ext/vector_float2.hpp"
#include "glm/fwd.hpp"

#include <glm/glm.hpp>

#include <signals.hpp>

#include <functional>
#include <vector>

enum class Origin { None = 0, Left = 1 << 1, Top = 1 << 2, Right = 1 << 3, Bottom = 1 << 4, Center = 1 << 5 };
inline Origin operator|(Origin a, Origin b) {
    return static_cast<Origin>(static_cast<int>(a) | static_cast<int>(b));
}
inline int operator&(Origin a, Origin b) {
    return static_cast<int>(a) & static_cast<int>(b);
}

class GUIComponent {
public:
    void Init();

    template <class T = GUIComponent>
    T *Add(Identifier const &identifier) {
        T *component = new T;
        component->m_Identifier = identifier;
        component->m_pParent = this;
        component->Init();

        m_pChildren.push_back(component);

        return component;
    }

    GUIComponent *GetParent();
    std::vector<GUIComponent *> &GetChildren();

    const Identifier &GetIdentifier() const {
        return m_Identifier;
    }

    const glm::mat4 &GetTransformation() const {
        return m_m4Transformation;
    }

    typename std::vector<GUIComponent *>::iterator begin() {
        return m_pChildren.begin();
    }
    typename std::vector<GUIComponent *>::iterator end() {
        return m_pChildren.end();
    }

    signals::signal<void(const glm::vec2 &v2Pos)> OnHover;
    signals::signal<void(const glm::vec2 &v2Pos)> OnHoverLost;

    signals::signal<void(float fDelta)> OnTick;
    signals::signal<void(float fDelta)> OnDraw;

    glm::vec2 m_v2Position{ 0.0f, 0.0f };

    Origin m_eOrigin = Origin::Top | Origin::Left;

    glm::vec2 m_v2Size{ 0.0f, 0.0f };
    glm::vec2 m_v2Scale{ 1.0f, 1.0f };
    float m_fRotation = 0.0f;

    glm::vec4 m_v4Color{ 1.0f, 1.0f, 1.0f, 1.0f };

    void CalculateTransformation() {
        auto offset = CalculateOffset();

        m_m4Transformation = glm::translate(glm::mat4(1.f), { m_v2Position, 1.0f });

        m_m4Transformation *= glm::translate(glm::mat4(1.f), { offset, 1.0f });
        m_m4Transformation *= glm::rotate(glm::mat4(1.f), glm::radians(m_fRotation), { 0.0f, 0.0f, 1.0f });
        m_m4Transformation *= glm::translate(glm::mat4(1.f), { -offset, 1.0f });

        m_m4Transformation *= glm::scale(glm::mat4(1.f), { m_v2Size * m_v2Scale, 1.0f });
    }

    Tweener<glm::vec2> *MoveTo(glm::vec2 const &v2Target) {
        return m_MoveTweener.From(m_v2Position)->To(v2Target);
    }

    Tweener<glm::vec2> *SizeTo(glm::vec2 const &v2Target) {
        return m_SizeTweener.From(m_v2Size)->To(v2Target);
    }

    Tweener<glm::vec2> *ScaleTo(glm::vec2 const &v2Target) {
        return m_ScaleTweener.From(m_v2Scale)->To(v2Target);
    }

    Tweener<float> *RotateTo(float fTarget) {
        return m_RotateTweener.From(m_fRotation)->To(fTarget);
    }

    Tweener<glm::vec4> *FadeTo(glm::vec4 const &v4Target) {
        return m_ColorTweener.From(m_v4Color)->To(v4Target);
    }

private:
    bool m_bIsHovering = false;

    glm::mat4 m_m4Transformation;

    glm::vec2 CalculateOffset();

    void Tick(float fDelta);
    void Draw(float fDelta);

    void OnMouseMove(const glm::vec2 &v2Pos);

    Identifier m_Identifier = "gui://empty";

    GUIComponent *m_pParent = nullptr;
    std::vector<GUIComponent *> m_pChildren{};

    Tweener<glm::vec2> m_MoveTweener{};
    Tweener<glm::vec2> m_SizeTweener{};
    Tweener<glm::vec2> m_ScaleTweener{};
    Tweener<float> m_RotateTweener{};
    Tweener<glm::vec4> m_ColorTweener{};
};
