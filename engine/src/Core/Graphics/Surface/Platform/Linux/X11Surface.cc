#include <cstring>

#include "X11Surface.hh"

#include "Core/Utils/Logger.hh"

#if PLATFORM_LINUX && !PLATFORM_ANDROID
    #define Font XID

    #include <X11/X.h>
    #include <X11/Xcursor/Xcursor.h>
    #include <X11/Xlib.h>
    #include <X11/keysym.h>

    // Fuck off Xlib
    #undef None

    #define DISPLAY (Display *) m_hDisplay
    #define WINDOW m_hWindow
    #define SCREEN m_hScreen

    // Window events
    #define UTF8_STRING XInternAtom( DISPLAY, "UTF8_STRING", False )

// clang-format off
    #define WM_NAME XInternAtom( DISPLAY, "_NET_WM_NAME", False )
    #define WM_VISIBLE_NAME XInternAtom( DISPLAY, "_NET_WM_VISIBLE_NAME", False )
    #define WM_ICON_NAME XInternAtom( DISPLAY, "_NET_WM_ICON_NAME", False )
    #define WM_VISIBLE_ICON_NAME XInternAtom( DISPLAY, "_NET_WM_VISIBLE_ICON_NAME", False )
    #define WM_DESKTOP XInternAtom( DISPLAY, "_NET_WM_DESKTOP", False )
    #define WM_WINDOW_TYPE XInternAtom( DISPLAY, "_NET_WM_WINDOW_TYPE", False )
// clang-format on

    #define WM_DELETE_WINDOW XInternAtom( DISPLAY, "WM_DELETE_WINDOW", False )

    #define SET_PROP( W, S )                                                             \
        XChangeProperty( DISPLAY, WINDOW, W, UTF8_STRING, 8, PropModeReplace,            \
                         (uint8_t *) S.c_str(), (int) S.size() );

    #define Button6 6
    #define Button7 7

Key TranslateXKeySym( KeySym keysyms )
{
    switch ( keysyms )
    {
    case XK_Escape: return Key::Key_ESCAPE;
    case XK_Tab: return Key::Key_TAB;
    case XK_Shift_L: return Key::Key_LEFT_SHIFT;
    case XK_Shift_R: return Key::Key_RIGHT_SHIFT;
    case XK_Control_L: return Key::Key_LEFT_CONTROL;
    case XK_Control_R: return Key::Key_RIGHT_CONTROL;
    case XK_Meta_L:
    case XK_Alt_L: return Key::Key_LEFT_ALT;
    case XK_Mode_switch:       // Mapped to Alt_R on many keyboards
    case XK_ISO_Level3_Shift:  // AltGr on at least some machines
    case XK_Meta_R:
    case XK_Alt_R: return Key::Key_RIGHT_ALT;
    case XK_Super_L: return Key::Key_LEFT_SUPER;
    case XK_Super_R: return Key::Key_RIGHT_SUPER;
    case XK_Menu: return Key::Key_MENU;
    case XK_Num_Lock: return Key::Key_NUM_LOCK;
    case XK_Caps_Lock: return Key::Key_CAPS_LOCK;
    case XK_Print: return Key::Key_PRINT_SCREEN;
    case XK_Scroll_Lock: return Key::Key_SCROLL_LOCK;
    case XK_Pause: return Key::Key_PAUSE;
    case XK_Delete: return Key::Key_DELETE;
    case XK_BackSpace: return Key::Key_BACKSPACE;
    case XK_Return: return Key::Key_ENTER;
    case XK_Home: return Key::Key_HOME;
    case XK_End: return Key::Key_END;
    case XK_Page_Up: return Key::Key_PAGE_UP;
    case XK_Page_Down: return Key::Key_PAGE_DOWN;
    case XK_Insert: return Key::Key_INSERT;
    case XK_Left: return Key::Key_LEFT;
    case XK_Right: return Key::Key_RIGHT;
    case XK_Down: return Key::Key_DOWN;
    case XK_Up: return Key::Key_UP;
    case XK_F1: return Key::Key_F1;
    case XK_F2: return Key::Key_F2;
    case XK_F3: return Key::Key_F3;
    case XK_F4: return Key::Key_F4;
    case XK_F5: return Key::Key_F5;
    case XK_F6: return Key::Key_F6;
    case XK_F7: return Key::Key_F7;
    case XK_F8: return Key::Key_F8;
    case XK_F9: return Key::Key_F9;
    case XK_F10: return Key::Key_F10;
    case XK_F11: return Key::Key_F11;
    case XK_F12: return Key::Key_F12;
    case XK_F13: return Key::Key_F13;
    case XK_F14: return Key::Key_F14;
    case XK_F15: return Key::Key_F15;
    case XK_F16: return Key::Key_F16;
    case XK_F17: return Key::Key_F17;
    case XK_F18: return Key::Key_F18;
    case XK_F19: return Key::Key_F19;
    case XK_F20: return Key::Key_F20;
    case XK_F21: return Key::Key_F21;
    case XK_F22: return Key::Key_F22;
    case XK_F23: return Key::Key_F23;
    case XK_F24: return Key::Key_F24;
    case XK_F25: return Key::Key_F25;

    // Numeric keypad
    case XK_KP_0: return Key::Key_KP_0;
    case XK_KP_1: return Key::Key_KP_1;
    case XK_KP_2: return Key::Key_KP_2;
    case XK_KP_3: return Key::Key_KP_3;
    case XK_KP_4: return Key::Key_KP_4;
    case XK_KP_5: return Key::Key_KP_5;
    case XK_KP_6: return Key::Key_KP_6;
    case XK_KP_7: return Key::Key_KP_7;
    case XK_KP_8: return Key::Key_KP_8;
    case XK_KP_9: return Key::Key_KP_9;
    case XK_KP_Separator:
    case XK_KP_Decimal: return Key::Key_KP_DECIMAL;

    case XK_KP_Divide: return Key::Key_KP_DIVIDE;
    case XK_KP_Multiply: return Key::Key_KP_MULTIPLY;
    case XK_KP_Subtract: return Key::Key_KP_SUBTRACT;
    case XK_KP_Add: return Key::Key_KP_ADD;

    case XK_KP_Insert: return Key::Key_KP_0;
    case XK_KP_End: return Key::Key_KP_1;
    case XK_KP_Down: return Key::Key_KP_2;
    case XK_KP_Page_Down: return Key::Key_KP_3;
    case XK_KP_Left: return Key::Key_KP_4;
    case XK_KP_Right: return Key::Key_KP_6;
    case XK_KP_Home: return Key::Key_KP_7;
    case XK_KP_Up: return Key::Key_KP_8;
    case XK_KP_Page_Up: return Key::Key_KP_9;
    case XK_KP_Delete: return Key::Key_KP_DECIMAL;
    case XK_KP_Equal: return Key::Key_KP_EQUAL;
    case XK_KP_Enter: return Key::Key_KP_ENTER;

    // Some keys are missing on other layouts
    case XK_a: return Key::Key_A;
    case XK_b: return Key::Key_B;
    case XK_c: return Key::Key_C;
    case XK_d: return Key::Key_D;
    case XK_e: return Key::Key_E;
    case XK_f: return Key::Key_F;
    case XK_g: return Key::Key_G;
    case XK_h: return Key::Key_H;
    case XK_i: return Key::Key_I;
    case XK_j: return Key::Key_J;
    case XK_k: return Key::Key_K;
    case XK_l: return Key::Key_L;
    case XK_m: return Key::Key_M;
    case XK_n: return Key::Key_N;
    case XK_o: return Key::Key_O;
    case XK_p: return Key::Key_P;
    case XK_q: return Key::Key_Q;
    case XK_r: return Key::Key_R;
    case XK_s: return Key::Key_S;
    case XK_t: return Key::Key_T;
    case XK_u: return Key::Key_U;
    case XK_v: return Key::Key_V;
    case XK_w: return Key::Key_W;
    case XK_x: return Key::Key_X;
    case XK_y: return Key::Key_Y;
    case XK_z: return Key::Key_Z;
    case XK_1: return Key::Key_1;
    case XK_2: return Key::Key_2;
    case XK_3: return Key::Key_3;
    case XK_4: return Key::Key_4;
    case XK_5: return Key::Key_5;
    case XK_6: return Key::Key_6;
    case XK_7: return Key::Key_7;
    case XK_8: return Key::Key_8;
    case XK_9: return Key::Key_9;
    case XK_0: return Key::Key_0;
    case XK_space: return Key::Key_SPACE;
    case XK_minus: return Key::Key_MINUS;
    case XK_equal: return Key::Key_EQUAL;
    case XK_bracketleft: return Key::Key_LEFT_BRACKET;
    case XK_bracketright: return Key::Key_RIGHT_BRACKET;
    case XK_backslash: return Key::Key_BACKSLASH;
    case XK_semicolon: return Key::Key_SEMICOLON;
    case XK_apostrophe: return Key::Key_APOSTROPHE;
    case XK_grave: return Key::Key_GRAVE_ACCENT;
    case XK_comma: return Key::Key_COMMA;
    case XK_period: return Key::Key_PERIOD;
    case XK_slash: return Key::Key_SLASH;
    case XK_less: return Key::Key_WORLD_1;
    default: break;
    }

    // No matching translation was found
    return Key::Key_UNKNOWN;
}

MouseButton TranslateXButton( uint32_t button )
{
    switch ( button )
    {
    case Button1: return MouseButton::BTN_1;
    case Button2: return MouseButton::BTN_2;
    case Button3: return MouseButton::BTN_3;
    case Button4: return MouseButton::BTN_4;
    case Button5: return MouseButton::BTN_5;
    case Button6: return MouseButton::BTN_6;
    case Button7: return MouseButton::BTN_7;
    };

    return MouseButton::BTN_1;
}

X11Surface::X11Surface( SurfaceDesc &desc ) : BaseSurface( desc )
{
    setlocale( LC_ALL, "" );

    Display *display = XOpenDisplay( NULL );
    if ( display == nullptr )
    {
        Console::Fatal( "Failed open X11 Display!" );
    }

    m_hDisplay = (uintptr_t) display;
    m_hScreen = DefaultScreen( display );
    m_hWindow = XCreateSimpleWindow(
        display, RootWindow( display, m_hScreen ), desc.ivWindowPos.x, desc.ivWindowPos.y,
        desc.ivWindowRes.x, desc.ivWindowRes.y, 1, BlackPixel( display, m_hScreen ),
        WhitePixel( display, m_hScreen ) );

    // clang-format off
    /*
    XSelectInput( display, m_hWindow,
                        KeyPressMask 
                      | KeyReleaseMask
                      | ButtonPressMask
                      | ButtonReleaseMask
                      | EnterWindowMask
                      | LeaveWindowMask
                      | PointerMotionMask
                      | PointerMotionHintMask
                      | Button1MotionMask
                      | Button2MotionMask
                      | Button3MotionMask
                      | Button4MotionMask
                      | Button5MotionMask
                      | ButtonMotionMask 
                      | KeymapStateMask 
                      | ExposureMask
                      | VisibilityChangeMask 
                      | StructureNotifyMask 
                      | ResizeRedirectMask
                      | SubstructureNotifyMask 
                      | SubstructureRedirectMask
                      | FocusChangeMask
                      | PropertyChangeMask 
                      | ColormapChangeMask
                      | OwnerGrabButtonMask );
                      */
    // clang-format on
    XMapWindow( display, m_hWindow );  // Bind our window to our surface
    XSetLocaleModifiers( "" );

    // Input
    XIM xim = XOpenIM( display, 0, 0, 0 );
    if ( !xim )
    {
        // fallback to internal input method
        XSetLocaleModifiers( "@im=none" );
        xim = XOpenIM( display, 0, 0, 0 );
    }

    XIC xic = XCreateIC( xim, XNInputStyle, XIMPreeditNothing | XIMStatusNothing,
                         XNClientWindow, m_hWindow, XNFocusWindow, m_hWindow, NULL );
    XSetICFocus( xic );
    XSelectInput( display, m_hWindow,
                  KeyPressMask | KeyReleaseMask | PointerMotionMask | FocusChangeMask
                      | ButtonPressMask | ButtonReleaseMask );

    // Register events
    Atom events [] = { WM_DELETE_WINDOW };
    XSetWMProtocols( display, m_hWindow, events, sizeof( events ) / sizeof( Atom ) );

    SetTitle( desc.sTitle );
}

X11Surface::~X11Surface()
{
    XCloseDisplay( DISPLAY );
}

void X11Surface::Poll()
{
    XEvent event;
    while ( XPending( DISPLAY ) )  // If we have any pending events, handle them
    {
        XNextEvent( DISPLAY, &event );

        switch ( event.type )
        {
        case ConfigureNotify:
        {
            XConfigureEvent &xce = event.xconfigure;

            // Resolution changed
            // FIXME: Fix whitescreen of death
            if ( xce.width != m_Desc.ivWindowRes.x && xce.height != m_Desc.ivWindowRes.y )
            {
                TranslateEvent( OSEventType::SIZE, COMBINEUSHORT( xce.width, xce.height ),
                                0 );
            }

            break;
        }
        case KeyRelease:
        case KeyPress:
        {
            XKeyEvent &xke = event.xkey;

            KeySym sym = XLookupKeysym( &xke, 0 );
            Key key = TranslateXKeySym( sym );  // Actual key

            KeyMod mod = KeyMod::None;

            switch ( key )
            {
            case Key::Key_RIGHT_SHIFT:
            case Key::Key_LEFT_SHIFT: mod |= KeyMod::SHIFT; break;

            case Key::Key_RIGHT_CONTROL:
            case Key::Key_LEFT_CONTROL: mod |= KeyMod::CONTROL; break;

            case Key::Key_RIGHT_ALT:
            case Key::Key_LEFT_ALT: mod |= KeyMod::ALT; break;

            case Key::Key_RIGHT_SUPER:
            case Key::Key_LEFT_SUPER: mod |= KeyMod::SUPER; break;

            case Key::Key_CAPS_LOCK: mod |= KeyMod::CAPS_LOCK; break;
            case Key::Key_NUM_LOCK: mod |= KeyMod::NUM_LOCK; break;

            default: break;
            }

            if ( mod != KeyMod::None )
            {
                m_iLastMod |= mod;

                if ( event.type == KeyRelease )
                {
                    m_iLastMod &= ~mod;
                }
            }

            OnSetKeyState(
                key,
                event.type == KeyPress ? ButtonState::Pressed : ButtonState::Released,
                m_iLastMod );

            break;
        }

        case ButtonPress:
        case ButtonRelease:
        {
            XButtonEvent &xke = event.xbutton;

            MouseButton button = TranslateXButton( xke.button );
            OnSetMouseState( button, event.type == ButtonPress ? ButtonState::Pressed
                                                               : ButtonState::Released );

            break;
        }

        case MotionNotify:
        {
            XMotionEvent &xme = event.xmotion;

            const int x = xme.x;
            const int y = xme.y;

            // FIXME: Only fire this if it's inside the window
            OnSetMousePosition( glm::ivec2 { x, y } );
            m_Desc.ivMouseRes.x = x;
            m_Desc.ivMouseRes.y = y;

            break;
        }

        case FocusIn: TranslateEvent( OSEventType::GAIN_FOCUS, 0, 0 ); break;

        case FocusOut: TranslateEvent( OSEventType::LOSE_FOCUS, 0, 0 ); break;

        case ClientMessage:
        {
            XClientMessageEvent &xme = event.xclient;
            if ( xme.data.l [ 0 ] == WM_DELETE_WINDOW )
            {
                TranslateEvent( OSEventType::QUIT, 0, 0 );
            }

            break;
        }
        }
    }
}

void X11Surface::SetTitle( const std::string &sTitle )
{
    SET_PROP( WM_NAME, sTitle );
    SET_PROP( WM_ICON_NAME, sTitle );
}

void X11Surface::SetPosition( const glm::ivec2 &ivPos )
{
    XMoveWindow( DISPLAY, WINDOW, ivPos.x, ivPos.y );
}

void X11Surface::SetResolution( const glm::ivec2 &ivRes )
{
    XResizeWindow( DISPLAY, WINDOW, ivRes.x, ivRes.y );
}

int X11Surface::GetMonitorWidth()
{
    XWindowAttributes attributes;
    XGetWindowAttributes( DISPLAY, RootWindow( DISPLAY, SCREEN ), &attributes );

    return attributes.width;
}

int X11Surface::GetMonitorHeight()
{
    XWindowAttributes attributes;
    XGetWindowAttributes( DISPLAY, RootWindow( DISPLAY, SCREEN ), &attributes );

    return attributes.height;
}

glm::ivec2 X11Surface::GetCursorPosition()
{
    return m_Desc.ivMouseRes;
}

void X11Surface::SetCursorPosition( const glm::ivec2 &ivPos )
{
    XSelectInput( DISPLAY, RootWindow( DISPLAY, WINDOW ), KeyReleaseMask );
    XWarpPointer( DISPLAY, 0, RootWindow( DISPLAY, WINDOW ), 0, 0, 0, 0, 100, 100 );
    XFlush( DISPLAY );
}

Cursor X11Surface::BlankCursor()
{
    static char data [ 1 ] = { 0 };  // 1 pxl transparent
    XColor dummy;

    Pixmap blank =
        XCreateBitmapFromData( DISPLAY, RootWindow( DISPLAY, WINDOW ), data, 1, 1 );
    if ( blank == 0 ) Console::Fatal( "Out of memory" );
    Cursor cursor = XCreatePixmapCursor( DISPLAY, blank, blank, &dummy, &dummy, 0, 0 );
    XFreePixmap( DISPLAY, blank );

    return cursor;
}

void X11Surface::SetCursor( SurfaceCursor eCursor )
{

    const char *cursor = "";

    switch ( eCursor )
    {
    case SurfaceCursor::Arrow: cursor = "arrow"; break;
    case SurfaceCursor::TextInput: cursor = "xterm"; break;

    case SurfaceCursor::ResizeAll: cursor = "cross"; break;
    case SurfaceCursor::ResizeEW: cursor = "sb_h_double_arrow"; break;
    case SurfaceCursor::ResizeNS: cursor = "sb_v_double_arrow"; break;

    case SurfaceCursor::ResizeNESW: cursor = "bottom_left_corner"; break;  // FIXME:
    case SurfaceCursor::ResizeNWSE: cursor = "bottom_right_corner"; break;

    case SurfaceCursor::Hand: cursor = "hand1"; break;
    case SurfaceCursor::NotAllowed: cursor = "X_cursor"; break;
    case SurfaceCursor::Hidden: cursor = ""; break;
    }

    Cursor c;
    if ( strcmp( cursor, "" ) == 0 )
    {
        c = BlankCursor();
    }
    else
    {
        c = XcursorLibraryLoadCursor( DISPLAY, cursor );
    }

    XDefineCursor( DISPLAY, WINDOW, c );
}

bgfx::PlatformData X11Surface::GetPlatformData()
{
    bgfx::PlatformData platformData;
    platformData.ndt = (void *) DISPLAY;
    platformData.nwh = (void *) WINDOW;

    return platformData;
}

#endif
