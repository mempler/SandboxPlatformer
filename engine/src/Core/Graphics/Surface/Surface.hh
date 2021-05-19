#pragma once

#include "Core/Managers/InputHelper.hh"

#include <vadefs.h>
#include <vcruntime.h>

// Cross platform windowing/surface implementation
// and specific OS/platform abstraction tool and utility
// damn, feel like doing professional stuff now. hire me valve

enum class eWindowFlags
{
    None,
    Fullscreen = 1 << 0,
    VSync = 1 << 1,
    Centered = 1 << 2,  // Desktop dist. specific
    Resizable = 1 << 3
};
EnumFlags( eWindowFlags );

struct SurfaceDesc
{
    std::string sTitle = "Noop";
    std::string sName = "IceApp";
    std::string sIcon = "";

    eWindowFlags eFlags = eWindowFlags::None;

    glm::ivec2 ivPos;
    glm::ivec2 ivRes;
};

// oh
#if PLATFORM_WIN32
typedef HWND SurfaceHandle;
#endif

enum class eOSEventType
{
    QUIT,
    GAIN_FOCUS,
    LOSE_FOCUS,
    LBUTTONDOWN,
    LBUTTONCLICK,
    RBUTTONDOWN,
    RBUTTONCLICK,
    SIZE,
};

//
// macro mess fuck to combine integer types
//
// uint16_t from left 2 bytes
#define ULVALUE( x ) ( (uint16_t) ( (uintptr_t) x & 0xffff ) )
// uint16_t from right 2 bytes
#define URVALUE( x ) ( (uint16_t) ( (uintptr_t) x >> 16 & 0xffff ) )
// combine 2 uint16_t into uint32_t
#define COMBINEUSHORT( a, b ) ( (uintptr_t) ( ULVALUE( a ) | ULVALUE( b ) << 16 ) )

// int16_t from left 2 bytes
#define SLVALUE( x ) ( (int16_t) ( (uintptr_t) x & 0xffff ) )
// int16_t from right 2 bytes
#define SRVALUE( x ) ( (int16_t) ( (uintptr_t) x >> 16 & 0xffff ) )
// combine 2 int16_t into int32_t
#define COMBINESSHORT( a, b ) ( (uintptr_t) ( SLVALUE( a ) | SLVALUE( b ) << 16 ) )

// left = signed, right = unsigned
#define COMBINESUSHORT( a, b ) ( (uintptr_t) ( SLVALUE( a ) | ULVALUE( b ) << 16 ) )
// left = unsigned, right = signed
#define COMBINEUSSHORT( a, b ) ( (uintptr_t) ( ULVALUE( a ) | SLVALUE( b ) << 16 ) )

class BaseSurface
{
  public:
    BaseSurface( SurfaceDesc &desc );
    virtual ~BaseSurface() {};

    SurfaceHandle GetHandle() const;

    uint32_t GetWidth();
    uint32_t GetHeight();

    bool ShouldExit();

    void TranslateEvent( eOSEventType eType, uintptr_t uLVal, intptr_t iRVal );

    virtual void Poll() = 0;

    virtual void SetTitle( const std::string &sTitle ) = 0;
    virtual void SetPosition( const glm::ivec2 &ivPos ) = 0;
    virtual void SetResolution( const glm::ivec2 &ivRes ) = 0;

    const virtual glm::ivec2 &GetMousePos() const = 0;

    // OS specific
    virtual int GetMonitorWidth() = 0;
    virtual int GetMonitorHeight() = 0;

  public:
    signals::signal<void( BaseSurface *, uint32_t, uint32_t )> OnResolutionChanged;
    signals::signal<void( Key, ButtonState, KeyMod )> OnSetKeyState;
    signals::signal<void( MouseButton, ButtonState, KeyMod )> OnSetMouseState;
    signals::signal<void()> OnLoseFocus;
    signals::signal<void()> OnGainFocus;

  protected:
    SurfaceHandle m_Handle;
    SurfaceDesc m_Desc;  // lets just store it there

    bool m_bExit = false;
};