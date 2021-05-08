#pragma once

#include "Easing.hh"

#include <iostream>
#include <vector>

template <typename T, typename F = float>
class Tweener {
public:
    Tweener *From(T from) {
        m_vTweenEvents.push_back({ from, T{}, EasingType::Linear, 0 });

        return this;
    }

    Tweener *To(T to) {
        m_vTweenEvents.at(m_vTweenEvents.size() - 1).m_To = to;

        return this;
    }

    Tweener *Within(F duration) {
        m_vTweenEvents.at(m_vTweenEvents.size() - 1).m_End = duration;

        return this;
    }

    Tweener *Easing(EasingType easing) {
        m_vTweenEvents.at(m_vTweenEvents.size() - 1).m_eType = easing;

        return this;
    }

    Tweener *Repeat() {
        m_vTweenEvents.push_back(m_vTweenEvents.at(m_vTweenEvents.size() - 1));

        return this;
    }

    bool HasNext() {
        return m_vTweenEvents.size() > 0;
    }

    T Current() {
        for (auto it = m_vTweenEvents.begin(); it != m_vTweenEvents.end();) {
            if (it->m_End > m_Current) {
                return m_LastValue = EaseValue(it->m_eType, it->m_From, it->m_To, m_Current / it->m_End);
            } else {
                m_Current = 0;
                it = m_vTweenEvents.erase(it);
            }
        }

        if (!HasNext())
            return m_LastValue;

        return m_LastValue = m_vTweenEvents.at(m_vTweenEvents.size() - 1).m_To;
    }

    void Tick(F delta) {
        m_Current += 1000 * delta;
    }

private:
    F m_Current = 0;

    T m_LastValue = {};

    struct TweenEvent {
        T m_From;
        T m_To;

        EasingType m_eType;

        F m_End;
    };

    std::vector<TweenEvent> m_vTweenEvents{};
};