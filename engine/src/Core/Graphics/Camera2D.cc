#include "pch.hh"

#include "Camera2D.hh"

#include "Core/Engine.hh"

#include "glm/common.hpp"

Camera2D::Camera2D( const glm::vec2 &v2Pos, const glm::vec2 &v2Scale ) :
    m_v2Pos( v2Pos ), m_v2Scale( v2Scale )
{
    ZoneScoped;

    CalculateMetrices();
}

void Camera2D::Init()
{
    ZoneScoped;

    // I wonder what would happen if this Camera gets destroyed...
    GetEngine()
        ->GetSurface()
        ->OnResolutionChanged.connect<&Camera2D::OnResolutionChanged>( this );
}

static glm::mat4 CalculateView( const glm::vec2 &v2Pos, float fRotation )
{
    ZoneScoped;

    return glm::inverse( glm::translate( glm::mat4( 1.f ),
                                         glm::ceil( glm::vec3( v2Pos.x, v2Pos.y, .0f ) ) )
                         * glm::rotate( glm::mat4( 1.f ), glm::radians( fRotation ),
                                        glm::vec3( .0f, .0f, 1.f ) ) );
}

static glm::mat4 CalculateProjection( const glm::vec2 &v2Scale )
{
    ZoneScoped;

    glm::mat4 proj = glm::ortho( 0.0f, glm::ceil( v2Scale.x ), glm::ceil( v2Scale.y ),
                                 .0f, .1f, 10000.f );
    proj [ 3 ].z = 1.f;
    return proj;
}

void Camera2D::CalculateMetrices()
{
    ZoneScoped;

    m_m4Projection = CalculateProjection( m_v2Scale );
    m_m4View = CalculateView( m_v2Pos, m_fRotation );
}

void Camera2D::SetPosition( const glm::vec2 &v2Pos )
{
    ZoneScoped;

    m_v2Pos = v2Pos;
    m_m4View = CalculateView( m_v2Pos, m_fRotation );
}

void Camera2D::SetScale( const glm::vec2 &v2Scale )
{
    ZoneScoped;

    m_v2Scale = v2Scale;
    m_m4Projection = CalculateProjection( m_v2Scale );
}

void Camera2D::SetZoom( float fPower )
{
    ZoneScoped;

    // Nothing, at least for now
}

void Camera2D::SetUniformTransform( bgfx::ViewId vViewID )
{
    ZoneScoped;

    bgfx::setViewTransform( vViewID, glm::value_ptr( m_m4View ),
                            glm::value_ptr( m_m4Projection ) );
}

bool Camera2D::IsInsideDrawArea( const glm::vec2 &v2Point ) noexcept
{
    auto &b = m_v2Pos;
    auto b2 = b + m_v2Scale;

    /* clang-format off */
    if ( v2Point.x >= b.x && v2Point.x <= b2.x
      && v2Point.y >= b.y && v2Point.y <= b2.y )
        return true;
    /* clang-format on */

    return false;
}

void Camera2D::OnResolutionChanged( BaseSurface *pGameWindow, uint32_t iWidth,
                                    uint32_t iHeight )
{
    ZoneScoped;

    // Update Projection Matrix
    SetScale( { iWidth, iHeight } );
}
