#pragma once

#include "glm/ext/matrix_float4x4.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/ext/vector_float2.hpp"
#include "glm/trigonometric.hpp"

inline glm::vec2 ApplyScale(glm::vec2 v2Target, glm::vec2 v2Original) {
    float targetAspect = v2Target.x / v2Target.y;
    float originalAspect = v2Original.x / v2Original.y;

    float scaleFactor = targetAspect > originalAspect ? v2Target.y / v2Original.y : v2Target.x / v2Original.x;

    return glm::vec2(scaleFactor);
}

namespace Math {
    inline glm::mat4 CalcTransform(const glm::vec3 &v3Position, const glm::vec2 &v2Size, float fRotation = 0.f, const glm::vec3 &v3RotationAxis = { 1, 0, 0 }) {
        return glm::translate(glm::mat4(1.f), glm::ceil(v3Position)) * glm::rotate(glm::mat4(1.f), glm::radians(fRotation), v3RotationAxis) *
               glm::scale(glm::mat4(1.f), { v2Size.x, v2Size.y, 1.f });
    }
} // namespace Math
