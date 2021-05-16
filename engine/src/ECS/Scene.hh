#pragma once

#include <string_view>

#include "Entity.hh"

#include "ISystem.hh"

#include "Core/Utils/Identifier.hh"

#include <entt.hpp>

#include <optional>

class Scene final
{
  public:
    explicit Scene();
    ~Scene();

    /*****************************************************
     * CreateEntity
     *
     * Creates an Entity which components can be attached
     * to. It'll be used to interact with the Engine
     * and non Engine systems
     *
     * @param identifier Identifier of the Entity
     *
     * @return Entity
     *****************************************************/
    Entity CreateEntity( Identifier const &identifier );

    /*****************************************************
     * RegisterSystem
     *
     * Registers a system, later to be run within
     * the engine itself to modify the current
     * workflow
     *****************************************************/
    template <typename Sys = ISystem>
    void RegisterSystem()
    {
        m_vpSystems.push_back( new Sys( GetRegistry() ) );
    }

    /*****************************************************
     * Tick
     *
     * Tick the underlying systems
     *
     * @param fDelta Delta time between the last tick
     *****************************************************/
    void Tick( float fDelta );

    /*****************************************************
     * Draw
     *
     * Draws the underlying systems
     *
     * @param fDelta Delta time between the last draw
     *****************************************************/
    void Draw( float fDelta );

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
    // We cannot store the system itself, as the type differs.
    std::vector<ISystem *> m_vpSystems {};

    entt::registry m_Registry {};
};
