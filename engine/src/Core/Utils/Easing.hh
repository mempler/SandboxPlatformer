#pragma once

#include "glm/common.hpp"

#include <glm/glm.hpp>
#include <glm/gtx/easing.hpp>

enum class EasingType {
    Linear,

    quadraticIn,
    quadraticOut,
    quadraticInOut,

    cubicIn,
    cubicOut,
    cubicInOut,

    quarticIn,
    quarticOut,
    quarticInOut,

    quinticIn,
    quinticOut,
    quinticInOut,

    sineIn,
    sineOut,
    sineInOut,

    circularIn,
    circularOut,
    circularInOut,

    exponentialIn,
    exponentialOut,
    exponentialInOut,

    elasticIn,
    elasticOut,
    elasticInOut,

    backIn,
    backOut,
    backInOut,

    bounceIn,
    bounceOut,
    bounceInOut,
};

template <typename T, typename F>
T EaseValue(EasingType eType, T a, T b, F t) {
    t = glm::clamp<F>(t, 0, 1);

    switch (eType) {
        case EasingType::Linear: return glm::mix(a, b, glm::linearInterpolation(t));

        case EasingType::quadraticIn: return glm::mix(a, b, glm::quadraticEaseIn(t));
        case EasingType::quadraticOut: return glm::mix(a, b, glm::quadraticEaseOut(t));
        case EasingType::quadraticInOut: return glm::mix(a, b, glm::quadraticEaseInOut(t));

        case EasingType::cubicIn: return glm::mix(a, b, glm::cubicEaseIn(t));
        case EasingType::cubicOut: return glm::mix(a, b, glm::cubicEaseOut(t));
        case EasingType::cubicInOut: return glm::mix(a, b, glm::cubicEaseInOut(t));

        case EasingType::quarticIn: return glm::mix(a, b, glm::quarticEaseIn(t));
        case EasingType::quarticOut: return glm::mix(a, b, glm::quarticEaseOut(t));
        case EasingType::quarticInOut: return glm::mix(a, b, glm::quarticEaseInOut(t));

        case EasingType::quinticIn: return glm::mix(a, b, glm::quinticEaseIn(t));
        case EasingType::quinticOut: return glm::mix(a, b, glm::quinticEaseOut(t));
        case EasingType::quinticInOut: return glm::mix(a, b, glm::quinticEaseInOut(t));

        case EasingType::sineIn: return glm::mix(a, b, glm::sineEaseIn(t));
        case EasingType::sineOut: return glm::mix(a, b, glm::sineEaseOut(t));
        case EasingType::sineInOut: return glm::mix(a, b, glm::sineEaseInOut(t));

        case EasingType::circularIn: return glm::mix(a, b, glm::circularEaseIn(t));
        case EasingType::circularOut: return glm::mix(a, b, glm::circularEaseOut(t));
        case EasingType::circularInOut: return glm::mix(a, b, glm::circularEaseInOut(t));

        case EasingType::exponentialIn: return glm::mix(a, b, glm::exponentialEaseIn(t));
        case EasingType::exponentialOut: return glm::mix(a, b, glm::exponentialEaseOut(t));
        case EasingType::exponentialInOut: return glm::mix(a, b, glm::exponentialEaseInOut(t));

        case EasingType::elasticIn: return glm::mix(a, b, glm::elasticEaseIn(t));
        case EasingType::elasticOut: return glm::mix(a, b, glm::elasticEaseOut(t));
        case EasingType::elasticInOut: return glm::mix(a, b, glm::elasticEaseInOut(t));

        case EasingType::backIn: return glm::mix(a, b, glm::backEaseIn(t));
        case EasingType::backOut: return glm::mix(a, b, glm::backEaseOut(t));
        case EasingType::backInOut: return glm::mix(a, b, glm::backEaseInOut(t));

        case EasingType::bounceIn: return glm::mix(a, b, glm::bounceEaseIn(t));
        case EasingType::bounceOut: return glm::mix(a, b, glm::bounceEaseOut(t));
        case EasingType::bounceInOut: return glm::mix(a, b, glm::bounceEaseInOut(t));
    }

    return {};
}
