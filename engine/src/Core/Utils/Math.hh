#pragma once

#include "glm/glm.hpp"

inline glm::vec2 ApplyScale(glm::vec2 v2Target, glm::vec2 v2Original) {
    float targetAspect = v2Target.x / v2Target.y;
    float originalAspect = v2Original.x / v2Original.y;

    float scaleFactor = targetAspect > originalAspect ? v2Target.y / v2Original.y : v2Target.x / v2Original.x;

    return glm::vec2(scaleFactor);
}
