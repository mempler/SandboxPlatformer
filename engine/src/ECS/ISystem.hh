#pragma once

#include <entt.hpp>

#include <memory>

class ISystem
{
  public:
    explicit ISystem( entt::registry *pRegistry ) : m_pRegistry( pRegistry )
    {
    }

    virtual ~ISystem() = default;

    /*****************************************************
     * Tick
     *
     * Tick the current system
     *
     * @param fDelta Delta time between the last tick
     *****************************************************/
    virtual void Tick( float fDelta ) = 0;

    /*****************************************************
     * Draw
     *
     * Draws the current system
     *
     * @param fDelta Delta time between the last draw
     *****************************************************/
    virtual void Draw( float fDelta ) = 0;

  protected:
    entt::registry *m_pRegistry;
};
