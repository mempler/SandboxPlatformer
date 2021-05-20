#include "TransformSystem.hh"

#include "entt/entity/fwd.hpp"

#include <IceSDK/ECS.hh>
#include <glm/ext/matrix_transform.hpp>

using namespace IceSDK;

void _internal::TransformSystem( float fDelta, Scene &scene, entt::registry &registry )
{
    auto group = registry.view<Components::Transform>();

    for ( entt::entity rawEntity : group )
    {
        Entity entity = scene.GetEntity( rawEntity );
        auto &component = entity.GetComponent<Components::Transform>();

        if ( !component.HasChanged ) continue;  // TODO: Check for cache miss ?

        // Once our entity has changed, we simply re-calculate the matrix

        glm::mat4 matrix { 1.0f };

        matrix = glm::translate( matrix, component.m_v3Position );
        matrix = glm::rotate( matrix, glm::radians( component.m_fRotation ),
                              glm::vec3( 0.f, 0.f, 1.f ) );
        matrix = glm::scale( matrix, component.m_v3Scale );

        component.m_m4ModelMatrix = matrix;
        component.HasChanged = false;  // Make sure we dont update again
    }
}
