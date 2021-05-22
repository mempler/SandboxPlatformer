#include <cwctype>

#include "Win32Surface.hh"

#include "Core/Engine.hh"
#include "Core/Graphics/Surface/Surface.hh"
#include "Core/Managers/InputHelper.hh"

#include <stdint.h>

#if PLATFORM_WIN32

    #include <dwmapi.h>
    #include <vadefs.h>

constexpr DWORD g_defWindowStyle =
    ( WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_VISIBLE | WS_POPUP
      | WS_VISIBLE );

Win32Surface::Win32Surface( SurfaceDesc &desc ) : BaseSurface( desc )
{
    m_Instance = GetModuleHandle( 0 );

    WNDCLASSEX wc;
    ZeroMemory( &wc, sizeof( WNDCLASSEX ) );

    wc.cbSize = sizeof( WNDCLASSEX );
    wc.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
    wc.lpfnWndProc = WindowProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = m_Instance;
    wc.hIcon = LoadIcon( 0, IDI_APPLICATION );
    wc.hCursor = LoadCursor( 0, IDC_ARROW );
    wc.hbrBackground = (HBRUSH) GetStockObject( BLACK_BRUSH );
    wc.lpszMenuName = 0;
    wc.lpszClassName = m_Desc.sName.c_str();
    wc.hIconSm = LoadIcon( 0, IDI_APPLICATION );
    // wc.hIcon = m_Desc.sIcon.c_str();

    RegisterClassEx( &wc );

    int windowFlags = g_defWindowStyle;

    if ( m_Desc.eFlags & WindowFlags::Fullscreen )
    {
        // window will be fullscreen, no need to calculate other position and stuff
        // just directly pass it over
        DEVMODE dm;
        ZeroMemory( &dm, sizeof( DEVMODE ) );

        dm.dmSize = sizeof( DEVMODE );
        dm.dmPelsWidth = GetMonitorWidth();
        dm.dmPelsHeight = GetMonitorHeight();
        dm.dmBitsPerPel = 32;
        dm.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

        // actual holy shit moment
        if ( ( m_Desc.ivWindowRes.x != GetMonitorWidth() )
             && ( m_Desc.ivWindowRes.y != GetMonitorHeight() ) )
        {
            if ( ChangeDisplaySettingsA( &dm, CDS_FULLSCREEN ) != DISP_CHANGE_SUCCESSFUL )
            {
                // throw error
                Console::Error( "Fullscreen is not supported by the GPU." );
            }
        }

        m_Handle =
            CreateWindowEx( 0, wc.lpszClassName, m_Desc.sTitle.c_str(),
                            WS_POPUP | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN, 0,
                            0, GetMonitorWidth(), GetMonitorHeight(), 0, 0, 0, 0 );
    }

    else
    {
        if ( m_Desc.eFlags & WindowFlags::Resizable )
            windowFlags |= WS_MAXIMIZEBOX | WS_THICKFRAME;

        int windowPosX = 0;
        int windowPosY = 0;

        if ( m_Desc.eFlags & WindowFlags::Centered )
        {
            windowPosX = ( GetMonitorWidth() / 2 ) - ( m_Desc.ivWindowRes.x / 2 );
            windowPosY = ( GetMonitorHeight() / 2 ) - ( m_Desc.ivWindowRes.y / 2 );
        }

        RECT rc = { 0, 0, m_Desc.ivWindowRes.x, m_Desc.ivWindowRes.y };

        AdjustWindowRectEx( &rc, windowFlags, 0, WS_EX_APPWINDOW | WS_EX_WINDOWEDGE );

        m_Handle = CreateWindowEx( 0, wc.lpszClassName, m_Desc.sTitle.c_str(),
                                   windowFlags, windowPosX, windowPosY,
                                   rc.right - rc.left, rc.bottom - rc.top, 0, 0, 0, 0 );
    }

    ShowWindow( m_Handle, SW_SHOW );
    UpdateWindow( m_Handle );
}

Win32Surface::~Win32Surface()
{
}

void Win32Surface::Poll()
{
    MSG msg;
    while ( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) )
    {
        TranslateMessage( &msg );
        DispatchMessage( &msg );
    }
}

void Win32Surface::SetTitle( const std::string &sTitle )
{
}

void Win32Surface::SetPosition( const glm::ivec2 &ivPos )
{
}

void Win32Surface::SetResolution( const glm::ivec2 &ivRes )
{
    m_Desc.ivWindowRes = ivRes;
}

Key TranslateWin32KeySym( WPARAM wParam )
{
    // 12345... + QWERTY...
    if ( wParam >= 0x30 && wParam <= 0x5A ) return (Key) wParam;
    // NUMPAD0-9 + /*-+
    if ( wParam >= 0x60 && wParam <= 0x69 ) return (Key) ( wParam + 224 );

    switch ( wParam )
    {
    case VK_ESCAPE: return Key::Key_ESCAPE;
    case VK_TAB: return Key::Key_TAB;
    case VK_LSHIFT: return Key::Key_LEFT_SHIFT;
    case VK_RSHIFT: return Key::Key_RIGHT_SHIFT;
    case VK_LCONTROL: return Key::Key_LEFT_CONTROL;
    case VK_RCONTROL: return Key::Key_RIGHT_CONTROL;
    case VK_LMENU: return Key::Key_LEFT_ALT;
    case VK_RMENU: return Key::Key_RIGHT_ALT;
    case VK_NUMLOCK: return Key::Key_NUM_LOCK;
    case VK_CAPITAL: return Key::Key_CAPS_LOCK;
    case VK_SNAPSHOT: return Key::Key_PRINT_SCREEN;
    case VK_SCROLL: return Key::Key_SCROLL_LOCK;
    case VK_PAUSE: return Key::Key_PAUSE;
    case VK_DELETE: return Key::Key_DELETE;
    case VK_BACK: return Key::Key_BACKSPACE;
    case VK_RETURN: return Key::Key_ENTER;
    case VK_HOME: return Key::Key_HOME;
    case VK_END: return Key::Key_END;
    case VK_PRIOR: return Key::Key_PAGE_UP;
    case VK_NEXT: return Key::Key_PAGE_DOWN;
    case VK_LEFT: return Key::Key_LEFT;
    case VK_RIGHT: return Key::Key_RIGHT;
    case VK_DOWN: return Key::Key_DOWN;
    case VK_UP: return Key::Key_UP;
    case VK_SPACE: return Key::Key_SPACE;
    case VK_LWIN: return Key::Key_LEFT_SUPER;
    case VK_RWIN: return Key::Key_RIGHT_SUPER;
    case VK_APPS: return Key::Key_MENU;
    case VK_INSERT: return Key::Key_INSERT;

    case VK_ADD: return Key::Key_KP_ADD;
    case VK_DECIMAL:
    case VK_SEPARATOR: return Key::Key_KP_DECIMAL;
    case VK_DIVIDE: return Key::Key_KP_DIVIDE;
    case VK_MULTIPLY: return Key::Key_KP_MULTIPLY;
    case VK_SUBTRACT: return Key::Key_KP_SUBTRACT;

    case VK_F1: return Key::Key_F1;
    case VK_F2: return Key::Key_F2;
    case VK_F3: return Key::Key_F3;
    case VK_F4: return Key::Key_F4;
    case VK_F5: return Key::Key_F5;
    case VK_F6: return Key::Key_F6;
    case VK_F7: return Key::Key_F7;
    case VK_F8: return Key::Key_F8;
    case VK_F9: return Key::Key_F9;
    case VK_F10: return Key::Key_F10;
    case VK_F11: return Key::Key_F11;
    case VK_F12: return Key::Key_F12;
    case VK_F13: return Key::Key_F13;
    case VK_F14: return Key::Key_F14;
    case VK_F15: return Key::Key_F15;
    case VK_F16: return Key::Key_F16;
    case VK_F17: return Key::Key_F17;
    case VK_F18: return Key::Key_F18;
    case VK_F19: return Key::Key_F19;
    case VK_F20: return Key::Key_F20;
    case VK_F21: return Key::Key_F21;
    case VK_F22: return Key::Key_F22;
    case VK_F23: return Key::Key_F23;
    case VK_F24: return Key::Key_F24;
    }

    return Key::Key_UNKNOWN;
}

Key TranslateWin32KeySym( WPARAM wParam )
{
    // 12345... + QWERTY...
    if ( wParam >= 0x30 || wParam <= 0x5A ) return (Key) wParam;
    // NUMPAD0-9 + /*-+
    if ( wParam >= 0x60 || wParam <= 0x69 ) return (Key) ( wParam + 224 );
    switch ( wParam )
    {
    case VK_ESCAPE: return Key::Key_ESCAPE;
    case VK_TAB: return Key::Key_TAB;
    case VK_LSHIFT: return Key::Key_LEFT_SHIFT;
    case VK_RSHIFT: return Key::Key_RIGHT_SHIFT;
    case VK_LCONTROL: return Key::Key_LEFT_CONTROL;
    case VK_RCONTROL: return Key::Key_RIGHT_CONTROL;
    case VK_LMENU: return Key::Key_LEFT_ALT;
    case VK_RMENU: return Key::Key_RIGHT_ALT;
    case VK_NUMLOCK: return Key::Key_NUM_LOCK;
    case VK_CAPITAL: return Key::Key_CAPS_LOCK;
    case VK_SNAPSHOT: return Key::Key_PRINT_SCREEN;
    case VK_SCROLL: return Key::Key_SCROLL_LOCK;
    case VK_PAUSE: return Key::Key_PAUSE;
    case VK_DELETE: return Key::Key_DELETE;
    case VK_BACK: return Key::Key_BACKSPACE;
    case VK_RETURN: return Key::Key_ENTER;
    case VK_HOME: return Key::Key_HOME;
    case VK_END: return Key::Key_END;
    case VK_PRIOR: return Key::Key_PAGE_UP;
    case VK_NEXT: return Key::Key_PAGE_DOWN;
    case VK_LEFT: return Key::Key_LEFT;
    case VK_RIGHT: return Key::Key_RIGHT;
    case VK_DOWN: return Key::Key_DOWN;
    case VK_UP: return Key::Key_UP;
    case VK_SPACE: return Key::Key_SPACE;
    case VK_LWIN: return Key::Key_LEFT_SUPER;
    case VK_RWIN: return Key::Key_RIGHT_SUPER;
    case VK_APPS: return Key::Key_MENU;
    case VK_INSERT: return Key::Key_INSERT;

    case VK_ADD: return Key::Key_KP_ADD;
    case VK_DECIMAL:
    case VK_SEPARATOR: return Key::Key_KP_DECIMAL;
    case VK_DIVIDE: return Key::Key_KP_DIVIDE;
    case VK_MULTIPLY: return Key::Key_KP_MULTIPLY;
    case VK_SUBTRACT: return Key::Key_KP_SUBTRACT;

    case VK_F1: return Key::Key_F1;
    case VK_F2: return Key::Key_F2;
    case VK_F3: return Key::Key_F3;
    case VK_F4: return Key::Key_F4;
    case VK_F5: return Key::Key_F5;
    case VK_F6: return Key::Key_F6;
    case VK_F7: return Key::Key_F7;
    case VK_F8: return Key::Key_F8;
    case VK_F9: return Key::Key_F9;
    case VK_F10: return Key::Key_F10;
    case VK_F11: return Key::Key_F11;
    case VK_F12: return Key::Key_F12;
    case VK_F13: return Key::Key_F13;
    case VK_F14: return Key::Key_F14;
    case VK_F15: return Key::Key_F15;
    case VK_F16: return Key::Key_F16;
    case VK_F17: return Key::Key_F17;
    case VK_F18: return Key::Key_F18;
    case VK_F19: return Key::Key_F19;
    case VK_F20: return Key::Key_F20;
    case VK_F21: return Key::Key_F21;
    case VK_F22: return Key::Key_F22;
    case VK_F23: return Key::Key_F23;
    case VK_F24: return Key::Key_F24;
    }

    return Key::Key_UNKNOWN;
}

LRESULT CALLBACK Win32Surface::WindowProc( HWND hwnd, UINT msg, WPARAM wParam,
                                           LPARAM lParam )
{
    if ( !GetApp() || !GetEngine() || !GetEngine()->GetSurface() )
        return DefWindowProc( hwnd, msg, wParam, lParam );

    BaseSurface *pSurf = GetEngine()->GetSurface();

    switch ( msg )
    {
    case WM_DESTROY:
    case WM_CLOSE: pSurf->TranslateEvent( OSEventType::QUIT, 0, 0 ); break;
    case WM_KILLFOCUS: pSurf->TranslateEvent( OSEventType::LOSE_FOCUS, 0, 0 ); break;
    case WM_ACTIVATEAPP: pSurf->TranslateEvent( OSEventType::GAIN_FOCUS, 0, 0 ); break;
    case WM_LBUTTONDOWN:
    case WM_LBUTTONDBLCLK:
    case WM_RBUTTONDOWN:
    case WM_RBUTTONDBLCLK:
    case WM_MOUSEMOVE:
    {
        MouseButton keys = MouseButton::NONE;
        if ( wParam & MK_LBUTTON ) keys |= MouseButton::BTN_1;
        if ( wParam & MK_RBUTTON ) keys |= MouseButton::BTN_2;

        KeyMod mods = KeyMod::None;

        if ( GetKeyState( VK_SHIFT ) & 0x8000 ) mods |= KeyMod::SHIFT;
        if ( GetKeyState( VK_CONTROL ) & 0x8000 ) mods |= KeyMod::CONTROL;
        if ( GetKeyState( VK_MENU ) & 0x8000 ) mods |= KeyMod::ALT;
        if ( GetKeyState( VK_LWIN ) & 0x8000 ) mods |= KeyMod::SUPER;
        if ( GetKeyState( VK_CAPITAL ) & 0x8000 ) mods |= KeyMod::CAPS_LOCK;

        uintptr_t uLVal = COMBINEUSHORT( keys, mods );

        pSurf->OnSetMouseState( keys, ButtonState::Pressed );

        if ( msg == WM_LBUTTONDOWN || msg == WM_RBUTTONDOWN )
            pSurf->TranslateEvent( OSEventType::MOUSE_DOWN, uLVal, lParam );

        else
            pSurf->TranslateEvent( OSEventType::MOUSE_DOUBLE_CLICK, uLVal, lParam );

        break;
    }
    case WM_LBUTTONUP:
    case WM_RBUTTONUP:
    {
        MouseButton keys = MouseButton::NONE;
        if ( wParam & MK_LBUTTON ) keys |= MouseButton::BTN_1;
        if ( wParam & MK_RBUTTON ) keys |= MouseButton::BTN_2;

        KeyMod mods = KeyMod::None;

        if ( GetKeyState( VK_SHIFT ) & 0x8000 ) mods |= KeyMod::SHIFT;
        if ( GetKeyState( VK_CONTROL ) & 0x8000 ) mods |= KeyMod::CONTROL;
        if ( GetKeyState( VK_MENU ) & 0x8000 ) mods |= KeyMod::ALT;
        if ( GetKeyState( VK_LWIN ) & 0x8000 ) mods |= KeyMod::SUPER;
        if ( GetKeyState( VK_CAPITAL ) & 0x8000 ) mods |= KeyMod::CAPS_LOCK;

        uintptr_t uLVal = COMBINEUSHORT( keys, mods );

        pSurf->OnSetMouseState( keys, ButtonState::Released );
        pSurf->TranslateEvent( OSEventType::MOUSE_UP, uLVal, lParam );
        break;
    }
    case WM_MOUSEMOVE:
    {
        pSurf->SetCursorPosition( { SLVALUE( lParam ), SRVALUE( lParam ) } );
        pSurf->TranslateEvent( OSEventType::MOUSE_MOVE, 0, 0 );
        break;
    }
    case WM_SIZE: pSurf->TranslateEvent( OSEventType::SIZE, lParam, wParam ); break;
    case WM_KEYUP:
    case WM_KEYDOWN:
    {
        Key key = TranslateWin32KeySym( wParam );

        KeyMod mods = KeyMod::None;

        if ( GetKeyState( VK_SHIFT ) & 0x8000 ) mods |= KeyMod::SHIFT;
        if ( GetKeyState( VK_CONTROL ) & 0x8000 ) mods |= KeyMod::CONTROL;
        if ( GetKeyState( VK_MENU ) & 0x8000 ) mods |= KeyMod::ALT;
        if ( GetKeyState( VK_LWIN ) & 0x8000 ) mods |= KeyMod::SUPER;
        if ( GetKeyState( VK_CAPITAL ) & 0x8000 ) mods |= KeyMod::CAPS_LOCK;

        pSurf->OnSetKeyState(
            key, msg == WM_KEYDOWN ? ButtonState::Pressed : ButtonState::Released, mods );

        pSurf->TranslateEvent( OSEventType::KEY_DOWN,
                               (uintptr_t) TranslateWin32KeySym( wParam ), lParam >> 30 );
        break;
    }
    case WM_CHAR:
    {
        // int uchar = 0;
        // uint8_t c = (uint8_t) wParam;
        // if ( !::IsWindowUnicode( hwnd ) )
        //     MultiByteToWideChar( CP_UTF8, 0, (LPCSTR) &c, 1, (LPWSTR) &uchar, 1 );

        pSurf->OnChar( wParam, KeyMod::None );
        break;
    }

    case WM_SETCURSOR:
    {
        pSurf->SetCursor( pSurf->m_eCurrentCursor );
        break;
    }

    default: return DefWindowProc( hwnd, msg, wParam, lParam );
    }
    return 0;
}

glm::ivec2 &Win32Surface::GetCursorPosition()
{
    return m_Desc.ivMousePos;
}

void Win32Surface::SetCursorPosition( const glm::ivec2 &ivPos )
{
    m_Desc.ivMousePos = ivPos;
    OnSetMousePosition( ivPos );
}

void Win32Surface::SetCursor( SurfaceCursor eCursor )
{
    ::ShowCursor( TRUE );  // make sure that cursor is shown

    static ::HCURSOR arrowCursor { LoadCursor( NULL, IDC_ARROW ) };
    static ::HCURSOR ibeamCursor { LoadCursor( NULL, IDC_IBEAM ) };
    static ::HCURSOR handCursor { LoadCursor( NULL, IDC_HAND ) };
    static ::HCURSOR sizeAllCursor { LoadCursor( NULL, IDC_SIZEALL ) };
    static ::HCURSOR sizeWECursor { LoadCursor( NULL, IDC_SIZEWE ) };
    static ::HCURSOR sizeNSCursor { LoadCursor( NULL, IDC_SIZENS ) };
    static ::HCURSOR sizeNESWCursor { LoadCursor( NULL, IDC_SIZENESW ) };
    static ::HCURSOR sizeNWSECursor { LoadCursor( NULL, IDC_SIZENWSE ) };
    static ::HCURSOR noCursor { LoadCursor( NULL, IDC_NO ) };

    switch ( eCursor )
    {
    case SurfaceCursor::Arrow: ::SetCursor( arrowCursor ); break;
    case SurfaceCursor::TextInput: ::SetCursor( ibeamCursor ); break;
    case SurfaceCursor::ResizeAll: ::SetCursor( sizeAllCursor ); break;
    case SurfaceCursor::ResizeEW: ::SetCursor( sizeWECursor ); break;
    case SurfaceCursor::ResizeNS: ::SetCursor( sizeNSCursor ); break;
    case SurfaceCursor::ResizeNESW: ::SetCursor( sizeNESWCursor ); break;  // FIXME:
    case SurfaceCursor::ResizeNWSE: ::SetCursor( sizeNWSECursor ); break;
    case SurfaceCursor::Hand: ::SetCursor( handCursor ); break;
    case SurfaceCursor::NotAllowed: ::SetCursor( noCursor ); break;
    case SurfaceCursor::Hidden: ::ShowCursor( FALSE ); break;
    }

    m_eCurrentCursor = eCursor;
}

int Win32Surface::GetMonitorWidth()
{
    return GetSystemMetrics( SM_CXSCREEN );
}

int Win32Surface::GetMonitorHeight()
{
    return GetSystemMetrics( SM_CYSCREEN );
}

bgfx::PlatformData Win32Surface::GetPlatformData()
{
    bgfx::PlatformData platformData;
    platformData.nwh = (void *) m_Handle;

    return platformData;
}

#endif