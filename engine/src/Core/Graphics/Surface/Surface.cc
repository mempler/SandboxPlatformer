#include "Surface.hh"

#include "Core/Engine.hh"

BaseSurface::BaseSurface( SurfaceDesc &desc ) : m_Desc( desc )
{
}

SurfaceHandle BaseSurface::GetHandle() const
{
    return m_Handle;
}

uint32_t BaseSurface::GetWidth()
{
    return m_Desc.ivRes.x;
}

uint32_t BaseSurface::GetHeight()
{
    return m_Desc.ivRes.y;
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
        Console::Info( "keys: {}, mods: {}, x: {}, y: {}", ULVALUE( uLVal ),
                       URVALUE( uLVal ), ULVALUE( iRVal ), URVALUE( iRVal ) );
        break;
    }
    case OSEventType::SIZE:
    {
        glm::ivec2 scl = { ULVALUE( uLVal ), URVALUE( uLVal ) };
        SetResolution( scl );

        OnResolutionChanged( this, scl.x, scl.y );
        bgfx::reset( scl.x, scl.y, GetEngine()->GetResetFlags() );
        GetEngine()->ResetTransform();
        Console::Info( "Window size changed to {}, {}.", scl.x, scl.y );

        break;
    }
    case OSEventType::KEY_DOWN: Console::Info( "keydown {}, {}.", (char)uLVal, (int)(char)iRVal ); break;

    default: break;
    }
}