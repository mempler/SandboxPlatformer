#include "Surface.hh"

#include "Core/Engine.hh"

BaseSurface::BaseSurface( SurfaceDesc &desc ) : m_Desc( desc )
{
}

SurfaceHandle BaseSurface::GetHandle() const
{
    return m_Handle;
}

bool BaseSurface::ShouldExit()
{
    return m_bExit;
}

void BaseSurface::TranslateEvent( OSEventType eType, uintptr_t uLVal, intptr_t iRVal )
{
    switch ( eType )
    {
    case OSEventType::QUIT: m_bExit = true; break;
    case OSEventType::GAIN_FOCUS: OnGainFocus(); break;
    case OSEventType::LOSE_FOCUS: OnLoseFocus(); break;
    case OSEventType::MOUSE_DOUBLE_CLICK:
    {
        break;
    }
    case OSEventType::SIZE:
    {
        glm::ivec2 scl = { ULVALUE( uLVal ), URVALUE( uLVal ) };
        SetResolution( scl );

        OnResolutionChanged( this, scl.x, scl.y );
        bgfx::reset( scl.x, scl.y, GetEngine()->GetResetFlags() );
        GetEngine()->ResetTransform();

        break;
    }
    case OSEventType::KEY_DOWN: break;

    default: break;
    }
}