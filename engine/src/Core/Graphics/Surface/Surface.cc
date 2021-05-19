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

void BaseSurface::TranslateEvent( eOSEventType eType, uintptr_t uLVal, intptr_t iRVal )
{
    switch ( eType )
    {
    case eOSEventType::QUIT: m_bExit = true; break;
    case eOSEventType::GAIN_FOCUS: OnGainFocus(); break;
    case eOSEventType::LOSE_FOCUS: OnLoseFocus(); break;
    case eOSEventType::LBUTTONCLICK:
    case eOSEventType::RBUTTONCLICK:
    case eOSEventType::LBUTTONDOWN:
    case eOSEventType::RBUTTONDOWN: break;
    // {
    //     bool button [ 2 ];
    //     button [ 0 ] = ( wParam & MK_LBUTTON ) != 0;
    //     button [ 1 ] = ( wParam & MK_RBUTTON ) != 0;

    //     KeyMod mods = KeyMod::None;

    //     if ( ( wParam & MK_SHIFT ) != 0 ) mods = (KeyMod) ( mods | KeyMod::SHIFT );
    //     if ( ( wParam & VK_SHIFT ) != 0 ) mods = (KeyMod) ( mods | KeyMod::CONTROL );

    //     for ( int i = 0; i < 2; i++ )
    //     {
    //         if ( button [ i ] )
    //         {
    //         }
    //     }
    //     break;
    // }
    case eOSEventType::SIZE:
    {
        glm::ivec2 scl = { ULVALUE( uLVal ), URVALUE( uLVal ) };
        SetResolution( scl );

        OnResolutionChanged( this, scl.x, scl.y );
        bgfx::reset( scl.x, scl.y, GetEngine()->GetResetFlags() );
        GetEngine()->ResetTransform();
        Console::Info( "Window size changed to {}, {}.", scl.x, scl.y );

        break;
    }
    default: break;
    }
}