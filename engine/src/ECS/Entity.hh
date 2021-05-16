#pragma once

#include <string_view>

#include "Kokoro/Exception/BaseException.hh"

#include <Kokoro/Exception.hh>
#include <glm/vec3.hpp>

#include <entt.hpp>

#define ENTITY_EXCEPTION( _FMT, ... )                                          \
    ::EntityException( fmt::format( _FMT, __VA_ARGS__ ), __FILE__, __LINE__ )

class EntityException final : public Kokoro::Exception::BaseException
{
  public:
    explicit EntityException( std::string_view svReason, const char *svWhere,
                              size_t sLine ) :
        BaseException( "Entity", svReason, svWhere, sLine )
    {
    }
};

class Entity final
{
  public:
    explicit Entity( entt::registry *wpRegistry ) : m_pRegistry( wpRegistry )
    {
        m_eHandle = wpRegistry->create();
    }

    explicit Entity( entt::registry *wpRegistry, entt::entity ehRaw ) :
        m_pRegistry( wpRegistry ), m_eHandle( ehRaw )
    {
    }

    explicit Entity()
    {
    }

    /*****************************************************
     * IsValid
     *
     * Tests if an Entity is valid
     *
     * @param eEntity Given entity
     *
     * @return True if valid
     *****************************************************/
    static bool IsValid( const Entity &eEntity )
    {
        return eEntity.m_eHandle != entt::null;
    }

    /*****************************************************
     * HasComponent<T>
     *
     * Checks if this entity has a component
     *
     * @return True if has
     *****************************************************/
    template <typename T>
    bool HasComponent()
    {
        return m_pRegistry->has<T>( m_eHandle );
    }

    /*****************************************************
     * GetComponent<T>
     *
     * Gets a specific component
     * Will fail if doesn't have
     *
     * @return True if has
     *****************************************************/
    template <typename T>
    T &GetComponent()
    {
        if ( !this->HasComponent<T>() )
            throw ENTITY_EXCEPTION(
                "Can't get a component, Entity({}) is missing it!", m_eHandle );

        return m_pRegistry->get<T>( m_eHandle );
    }

    /*****************************************************
     * AttachComponent<T>
     *
     * Attach a component
     * It'll fail if the component has already
     * been attached
     *
     * @return Reference to the attached component
     *****************************************************/
    template <typename T, typename... Args>
    T &AttachComponent( Args &&...tArgs )
    {
        if ( !this->HasComponent<T>() )
            throw ENTITY_EXCEPTION(
                "Can't attach a component, Entity({}) has it already!",
                m_eHandle );

        return m_pRegistry->emplace<T>( m_eHandle,
                                        std::forward<Args>( tArgs )... );
    }

    /*****************************************************
     * DetachComponent<T>
     *
     * Detaches a component, it won't fail if
     * it doesn't have it.
     *****************************************************/
    template <typename T>
    void DetachComponent()
    {
        m_pRegistry->remove_if_exists<T>( m_eHandle );
    }

    /*****************************************************
     * WithPosition
     *
     * Sets the position of an Entity using the TransformComponent
     *
     * @param v3Position Target Position
     *
     * @see Components::TransformComponent
     *
     * @return self
     *****************************************************/
    Entity &WithPosition( const glm::vec3 &v3Position );

    /*****************************************************
     * WithScale
     *
     * Sets the scale of an Entity using the TransformComponent
     *
     * @param v3Scale Target Scale
     *
     * @see Components::TransformComponent
     *
     * @return self
     *****************************************************/
    Entity &WithScale( const glm::vec3 &v3Scale );

    /*****************************************************
     * WithRotation
     *
     * Sets the rotation of an Entity using the TransformComponent
     *
     * @param fDegrees Target Rotation in degrees
     *
     * @see Components::TransformComponent
     *
     * @return self
     *****************************************************/
    Entity &WithRotation( const float fDegrees );

    /*****************************************************
     * DefaultTransformation
     *
     * Attaches a default TransformComponent if doesn't exist
     *
     * @see Components::TransformComponent
     *
     * @return self
     *****************************************************/
    Entity &DefaultTransformation();

  private:
    entt::registry *m_pRegistry;

    entt::entity m_eHandle = entt::null;
};
