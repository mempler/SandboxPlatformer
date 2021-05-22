#pragma once

#include <cstdint>
#include <string>

#include "Core/Managers/InputHelper.hh"

#include "glm/fwd.hpp"

#include <Kokoro/Utility/BitFlags.hh>
#include <bgfx/bgfx.h>
#include <glm/glm.hpp>

#include <signals.hpp>

// Cross platform windowing/surface implementation
// and specific OS/platform abstraction tool and utility
// damn, feel like doing professional stuff now. hire me valve

enum class WindowFlags
{
    None,
    Fullscreen = 1 << 0,
    VSync = 1 << 1,
    Centered = 1 << 2,  // Desktop dist. specific
    Resizable = 1 << 3
};
BitFlags( WindowFlags );

enum class SurfaceCursor
{
    Arrow,
    TextInput,
    ResizeAll,
    ResizeEW,
    ResizeNS,
    ResizeNESW,
    ResizeNWSE,
    Hand,
    NotAllowed,
    Hidden,
};

struct SurfaceDesc
{
    std::string sTitle = "Noop";
    std::string sName = "IceApp";
    std::string sIcon = "";

    WindowFlags eFlags = WindowFlags::None;

    glm::ivec2 ivWindowPos;
    glm::ivec2 ivWindowRes;

    glm::ivec2 ivMousePos;
};

typedef intptr_t SurfaceHandle;

enum class OSEventType
{
    QUIT,
    GAIN_FOCUS,
    LOSE_FOCUS,
    MOUSE_CLICK,
    MOUSE_DOWN,
    MOUSE_UP,
    MOUSE_MOVE,
    MOUSE_DOUBLE_CLICK,
    SIZE,
    SIZING,
    PAINT,
    ACTIVATE,
    KEY_DOWN,
    KEY_UP
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

    inline uint32_t GetWidth()
    {
        return m_Desc.ivWindowRes.x;
    }
    inline uint32_t GetHeight()
    {
        return m_Desc.ivWindowRes.y;
    }

    bool ShouldExit();

    void TranslateEvent( OSEventType eType, uintptr_t uLVal, intptr_t iRVal );

    virtual void Poll() = 0;

    virtual void SetTitle( const std::string &sTitle ) = 0;
    virtual void SetPosition( const glm::ivec2 &ivPos ) = 0;
    virtual void SetResolution( const glm::ivec2 &ivRes ) = 0;

    // OS specific
    virtual int GetMonitorWidth() = 0;
    virtual int GetMonitorHeight() = 0;

    virtual glm::ivec2 &GetCursorPosition() = 0;
    virtual void SetCursor( SurfaceCursor eCursor ) = 0;
    virtual void SetCursorPosition( const glm::ivec2 &ivPos ) = 0;

    virtual bgfx::PlatformData GetPlatformData() = 0;

  public:
    SurfaceCursor m_eCurrentCursor = SurfaceCursor::Arrow;

    KeyMod m_iLastMod = KeyMod::None;

    signals::signal<void( BaseSurface *, uint32_t, uint32_t )> OnResolutionChanged;
    signals::signal<void( Key, ButtonState, KeyMod )> OnSetKeyState;
    signals::signal<void( MouseButton, ButtonState )> OnSetMouseState;
    signals::signal<void( glm::ivec2 )> OnSetMousePosition;
    signals::signal<void()> OnLoseFocus;
    signals::signal<void()> OnGainFocus;

    signals::signal<void( uint32_t, KeyMod )> OnChar;  // Text input

  protected:
    SurfaceHandle m_Handle;
    SurfaceDesc m_Desc;  // lets just store it there

    bool m_bExit = false;
};