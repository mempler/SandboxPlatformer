#pragma once

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

namespace IceSDK::Components
{
    // Changing just those values wont update the model matrix!
    // Make sure to modify *HasChanged* to update the matrix cache.
    struct Transform
    {
        glm::vec3 m_v3Position;
        glm::vec3 m_v3Scale;
        float m_fRotation;

        glm::mat4 m_m4ModelMatrix;

        bool HasChanged = false;
    };
}  // namespace IceSDK::Components
