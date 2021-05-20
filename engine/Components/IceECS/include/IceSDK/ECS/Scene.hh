#pragma once

#include <functional>
#include <optional>
#include <string_view>

#include "Entity.hh"
#include "entt/entity/fwd.hpp"

#include <entt/entt.hpp>

namespace IceSDK
{
    class Scene final
    {
      public:
        typedef std::function<void( float, Scene &, entt::registry & )> SystemFuncType;

        /*****************************************************
         * CreateEntity
         *
         * Creates an Entity which components can be attached
         * to. It'll be used to interact with the Engine
         * and non Engine systems
         *
         * @return Entity
         *****************************************************/
        inline Entity CreateEntity()
        {
            return Entity( GetRegistry() );
        }

        /*****************************************************
         * RegisterDrawSystem
         *
         * Registers a system, later to be run within
         * the engine itself to modify the current
         * workflow
         *****************************************************/
        inline void RegisterDrawSystem( SystemFuncType fnc )
        {
            m_vDrawSystems.push_back( fnc );
        }

        /*****************************************************
         * RegisterTickSystem
         *
         * Registers a system, later to be run within
         * the engine itself to modify the current
         * workflow
         *****************************************************/
        inline void RegisterTickSystem( SystemFuncType fnc )
        {
            m_vTickSystems.push_back( fnc );
        }

        inline Entity GetEntity( entt::entity entity )
        {
            return Entity( GetRegistry(), entity );
        }

        /*****************************************************
         * Tick
         *
         * Tick the underlying systems
         *
         * @param fDelta Delta time between the last tick
         *****************************************************/
        void Tick( float fDelta )
        {
            for ( auto &system : m_vTickSystems )
            {
                system( fDelta, *this, m_Registry );
            }
        }

        /*****************************************************
         * Draw
         *
         * Draws the underlying systems
         *
         * @param fDelta Delta time between the last draw
         *****************************************************/
        void Draw( float fDelta )
        {
            for ( auto &system : m_vDrawSystems )
            {
                system( fDelta, *this, m_Registry );
            }
        }

        /*****************************************************
         * GetRegistry
         *
         * Gets the registry to interact with entities
         *
         * @return A raw ptr towards registry
         *         This might be null if Scene
         *         is no longer existent
         *****************************************************/
        entt::registry *GetRegistry()
        {
            return &m_Registry;
        }

      private:
        void RegisterDefaultSystems();

        // We cannot store the system itself, as the type differs.
        std::vector<SystemFuncType> m_vDrawSystems {};
        std::vector<SystemFuncType> m_vTickSystems {};

        entt::registry m_Registry {};
    };
}  // namespace IceSDK
