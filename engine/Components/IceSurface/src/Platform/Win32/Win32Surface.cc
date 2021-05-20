#include "Win32Surface.hh"

#if PLATFORM_WIN32

    #include <dwmapi.h>

constexpr DWORD g_defWindowStyle =
    ( WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_VISIBLE | WS_POPUP
      | WS_VISIBLE );

Win32Surface::Win32Surface( SurfaceDesc &desc ) : BaseSurface( desc )
{
    WNDCLASSEX wc;
    ZeroMemory( &wc, sizeof( WNDCLASSEX ) );

    wc.cbSize = sizeof( WNDCLASSEX );
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = WindowProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = 0;
    wc.hIcon = LoadIcon( 0, IDI_APPLICATION );
    wc.hCursor = LoadCursor( 0, IDC_ARROW );
    wc.hbrBackground = (HBRUSH) GetStockObject( BLACK_BRUSH );
    wc.lpszMenuName = 0;
    wc.lpszClassName = m_Desc.sName.c_str();
    wc.hIconSm = LoadIcon( 0, IDI_APPLICATION );
    // wc.hIcon = m_Desc.sIcon.c_str();

    RegisterClassEx( &wc );

    int windowFlags = g_defWindowStyle;

    if ( m_Desc.eFlags & eWindowFlags::Fullscreen )
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
        if ( ( m_Desc.ivRes.x != GetMonitorWidth() )
             && ( m_Desc.ivRes.y != GetMonitorHeight() ) )
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
        if ( m_Desc.eFlags & eWindowFlags::Resizable )
            windowFlags |= WS_MAXIMIZEBOX | WS_THICKFRAME;

        int windowPosX = 0;
        int windowPosY = 0;

        if ( m_Desc.eFlags & eWindowFlags::Centered )
        {
            windowPosX = ( GetMonitorWidth() / 2 ) - ( m_Desc.ivRes.x / 2 );
            windowPosY = ( GetMonitorHeight() / 2 ) - ( m_Desc.ivRes.y / 2 );
        }

        RECT rc = { 0, 0, m_Desc.ivRes.x, m_Desc.ivRes.y };

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
    m_Desc.ivRes = ivRes;
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
        pSurf->TranslateEvent( OSEventType::LBUTTONDOWN, lParam, wParam );
        break;
    case WM_LBUTTONDBLCLK:
        pSurf->TranslateEvent( OSEventType::LBUTTONCLICK, lParam, wParam );
        break;
    case WM_RBUTTONDOWN:
        pSurf->TranslateEvent( OSEventType::RBUTTONDOWN, lParam, wParam );
        break;
    case WM_RBUTTONDBLCLK:
        pSurf->TranslateEvent( OSEventType::RBUTTONCLICK, lParam, wParam );
        break;
    case WM_SIZE: pSurf->TranslateEvent( OSEventType::SIZE, lParam, wParam ); break;

    default: return DefWindowProc( hwnd, msg, wParam, lParam );
    }

    return 0;
}

int Win32Surface::GetMonitorWidth()
{
    return GetSystemMetrics( SM_CXSCREEN );
}

int Win32Surface::GetMonitorHeight()
{
    return GetSystemMetrics( SM_CYSCREEN );
}

void Win32Surface::AdjustViewRect( RECT &rect )
{
}

#endif