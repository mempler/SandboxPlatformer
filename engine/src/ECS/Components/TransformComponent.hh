#pragma once

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

namespace Components {
    struct TransformComponent {
        // Changing just those values wont update the model matrix!
        // Make sure to call UpdateModelMatrix() to update the matrix cache.
        glm::vec3 m_v3Position;
        glm::vec3 m_v3Scale;
        float m_fRotation;

        glm::mat4 m_m4ModelMatrix;

        bool _hasChanged = false;
        void UpdateModelMatrix() {
            _hasChanged = true;
        }
    };
} // namespace Components
