#pragma once

#include <string_view>

#include <Kokoro/Exception.hh>
#include <entt/entt.hpp>
#include <glm/vec3.hpp>

#define ENTITY_EXCEPTION( _FMT, ... )                                                    \
    ::IceSDK::EntityException( fmt::format( _FMT, __VA_ARGS__ ), __FILE__, __LINE__ )

namespace IceSDK
{
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
        explicit Entity( entt::registry *pRegistry ) : m_pRegistry( pRegistry )
        {
            m_eHandle = m_pRegistry->create();
        }

        explicit Entity( entt::registry *pRegistry, entt::entity ehRaw ) :
            m_pRegistry( pRegistry ), m_eHandle( ehRaw )
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
        inline static bool IsValid( const Entity &eEntity )
        {
            if ( eEntity.m_pRegistry != nullptr )
                return eEntity.m_pRegistry->valid( eEntity.m_eHandle );

            return eEntity.m_eHandle != entt::null;
        }

        /*****************************************************
         * HasComponents<T>
         *
         * Checks if this entity has following components
         *
         * @return True if has
         *****************************************************/
        template <typename... T>
        inline bool HasComponents()
        {
            return m_pRegistry->all_of<T...>( m_eHandle );
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
        inline T &GetComponent()
        {
            if ( !this->HasComponents<T>() )
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
        inline T &AttachComponent( Args &&...tArgs )
        {
            if ( !this->HasComponents<T>() )
                throw ENTITY_EXCEPTION(
                    "Can't attach a component, Entity({}) has it already!", m_eHandle );

            return m_pRegistry->emplace<T>( m_eHandle, std::forward<Args>( tArgs )... );
        }

        /*****************************************************
         * DetachComponent<T>
         *
         * Detaches a component, it won't fail if
         * it doesn't have it.
         *****************************************************/
        template <typename T>
        inline void DetachComponent()
        {
            m_pRegistry->remove<T>( m_eHandle );
        }

      private:
        entt::registry *m_pRegistry = nullptr;

        entt::entity m_eHandle = entt::null;
    };
}  // namespace IceSDK