#include "pch.hh"

#include "Entity.hh"

#include "Components/BaseComponent.hh"
#include "Components/TransformComponent.hh"

Entity &Entity::WithPosition( const glm::vec3 &v3Position )
{
    DefaultTransformation();  // Setup default transformation

    auto &transformComponent = GetComponent<Components::TransformComponent>();

    transformComponent.m_v3Position = v3Position;
    transformComponent.UpdateModelMatrix();

    return *this;
}

Entity &Entity::WithScale( const glm::vec3 &v3Scale )
{
    DefaultTransformation();  // Setup default transformation

    auto &transformComponent = GetComponent<Components::TransformComponent>();

    transformComponent.m_v3Scale = v3Scale;
    transformComponent.UpdateModelMatrix();

    return *this;
}

Entity &Entity::WithRotation( const float fDegrees )
{
    DefaultTransformation();  // Setup default transformation

    auto &transformComponent = GetComponent<Components::TransformComponent>();

    transformComponent.m_fRotation = fDegrees;
    transformComponent.UpdateModelMatrix();

    return *this;
}

Entity &Entity::DefaultTransformation()
{
    if ( HasComponent<Components::TransformComponent>() ) return *this;

    auto &transformComponent =
        AttachComponent<Components::TransformComponent>();

    transformComponent.m_fRotation = 0.f;
    transformComponent.m_v3Position = { 0.f, 0.f, 0.f };
    transformComponent.m_v3Scale = { 1.f, 1.f, 1.f };
    transformComponent.UpdateModelMatrix();

    return *this;
}
