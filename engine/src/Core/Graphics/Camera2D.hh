#pragma once

#include "Core/Graphics/Window.hh"

#include <bgfx/bgfx.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

class Camera2D
{
  public:
    Camera2D( const glm::vec2& v2Pos, const glm::vec2& v2Scale );

    void Init( );

    // Calculate camera projection and view.
    // Should not be spammed for each frame. Eats CPU a lot.
    // Prefer updating when position or scale changes.
    void CalculateMetrices( );

    // Updates position and metrices.
    void SetPosition( const glm::vec2& v2Pos );

    // Updates scale and metrices.
    void SetScale( const glm::vec2& v2Scale );

    // WARNING: Experimental function.
    // Not sure if it works or not, but it's
    // forced to zoom into center. Updates matrix
    // in each call. Do not spam.
    void SetZoom( float fPower );

    // Quick way of telling GPU to set transform.
    // Basic abstraction of bgfx::setViewTransform(...)
    void SetUniformTransform( bgfx::ViewId vViewID );

  public:
    // Returns constant pointer of projection.
    // This var cannot be edited. And shouldn't.
    const void* GetProjectionMatrixPtr( ) const
    {
        return glm::value_ptr( m_m4Projection );
    }

    // Returns constant pointer of view.
    // This var cannot be edited. And shouldn't.
    const void* GetViewMatrixPtr( ) const
    {
        return glm::value_ptr( m_m4View );
    }

    // Returns constant reference of position.
    // This var cannot be edited. And shouldn't.
    const glm::vec2& GetPosition( ) const
    {
        return m_v2Pos;
    }

    // Returns constant reference of scale.
    // This var cannot be edited. And shouldn't.
    const glm::vec2& GetScale( ) const
    {
        return m_v2Scale;
    }

  private:
    glm::mat4 m_m4Projection { };
    glm::mat4 m_m4View { };

    glm::vec2 m_v2Pos { };
    glm::vec2 m_v2Scale { };

    float m_fRotation { };
    float m_fZoom { };

  private:
    void OnResize( GameWindow* pGameWindow, uint32_t iWidth, uint32_t iHeight );
};