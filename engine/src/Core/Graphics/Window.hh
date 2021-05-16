#pragma once

#include "pch.hh"

#include "signals.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <SDL_events.h>

class ENGINE_EXPORT GameWindow
{
  public:
    enum Flags
    {
        None = 0,
        Fullscreen = 1 << 1,
        Unresizable = 1 << 2
    };

    /*****************************************************
     * GameWindow
     *
     * Create a cross-platform window to Render graphics
     * onto it.
     *
     * NOTE: It must be created on the Render thread!
     *
     * @param iWidth Window Width
     * @param iHeight Window Height
     * @param szTitle Window Title
     * @param eFlags Window Flags
     *****************************************************/
    explicit GameWindow(
        const int32_t iWidth = 1280, const int32_t iHeight = 720,
        const char *szTitle = "Sandbox Platformer: Game Window",
        const Flags eFlags = Flags::None );

    /*****************************************************
     * ~GameWindow
     *
     * Destroys the Window && Shutsdown BGFX
     *****************************************************/
    ~GameWindow();

    /*****************************************************
     * BeginRender
     *
     * Gets the current state and begins a new frame.
     *
     * NOTE: This must be called on the Render thread.
     *****************************************************/
    double BeginFrame();

    /*****************************************************
     * EndFrame
     *
     * Ends the current frame and flushes it to the Window
     *
     * NOTE: This must be called on the Render thread.
     *****************************************************/
    void EndFrame();

    /*****************************************************
     * ShouldExit
     *
     * Checks if this window should exit
     * cause E.G Exit() was called
     *
     * @return true if exit
     *****************************************************/
    bool ShouldExit();

    /*****************************************************
     * AddView
     *
     * Adds another view into view stack, useful when
     * updating all at once.
     *****************************************************/
    void AddView( bgfx::ViewId viID );

    /*****************************************************
     * ResetTransform
     *
     * Updates transform uniform for all views in stack.
     *****************************************************/
    void ResetTransform();

    bool CanRender()
    {
        return m_bIsBGFXInitialized;
    }

  public:
    void InitBGFX();

    int32_t Width()
    {
        return m_iWidth;
    }

    int32_t Height()
    {
        return m_iHeight;
    }

    // Yes, signals are public
    signals::signal<void( GameWindow *, SDL_Event & )> OnSDL2Event;
    signals::signal<void( GameWindow *, uint32_t, uint32_t )> OnResize;
    signals::signal<void( float )> OnUpdate;

  private:
    SDL_Window *m_SDLWindow = nullptr;

    int32_t m_iWidth = 0;
    int32_t m_iHeight = 0;

    bool m_bExit = false;
    bool m_bIsBGFXInitialized = false;

    int64_t m_iLastTime = 0;

    std::vector<bgfx::ViewId> m_vViews;
};