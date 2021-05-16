#pragma once

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

class BaseSurface
{
  public:
    BaseSurface( SurfaceDesc &desc );
    virtual ~BaseSurface() {};

    SurfaceHandle GetHandle() const;

    uint32_t GetWidth();
    uint32_t GetHeight();

    bool ShouldExit();

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
    signals::signal<void( int, int, int, int )> OnSetKeyState;
    signals::signal<void( int, int, int )> OnSetMouseState;
    signals::signal<void()> OnLoseFocus;
    signals::signal<void()> OnGainFocus;

  protected:
    SurfaceHandle m_Handle;
    SurfaceDesc m_Desc;  // lets just store it there

    bool m_bExit = false;
};