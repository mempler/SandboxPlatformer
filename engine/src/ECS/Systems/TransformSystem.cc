#include "pch.hh"

#include "ECS/Systems/TransformSystem.hh"

#include "ECS/Components/TransformComponent.hh"
#include "ECS/Entity.hh"

#include <glm/ext/matrix_transform.hpp>
#include <glm/glm.hpp>

using namespace Systems;

void TransformSystem::Draw( float pDelta )
{
}

void TransformSystem::Tick( float pDelta )
{
    auto transformGroup = m_pRegistry->view<Components::TransformComponent>();

    for ( auto rawTransformEntity : transformGroup )
    {
        auto spriteEntity = Entity( m_pRegistry, rawTransformEntity );
        auto &transform =
            spriteEntity.GetComponent<Components::TransformComponent>();

        if ( !transform._hasChanged ) continue;

        // Once our entity has changed, we simply re-calculate the matrix

        glm::mat4 matrix { 1.0f };

        matrix = glm::translate( matrix, transform.m_v3Position );
        matrix = glm::rotate( matrix, glm::radians( transform.m_fRotation ),
                              glm::vec3( 0.f, 0.f, 1.f ) );
        matrix = glm::scale( matrix, transform.m_v3Scale );

        transform.m_m4ModelMatrix = matrix;
        transform._hasChanged = false;
    }
}
