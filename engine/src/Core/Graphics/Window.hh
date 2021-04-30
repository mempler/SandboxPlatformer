#pragma once

#include "pch.hh"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

class ENGINE_EXPORT GameWindow {
public:
    enum Flags { None = 0, Fullscreen = 1 << 1, Unresizable = 1 << 2 };

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
        const int32_t iWidth = 1280, const int32_t iHeight = 720, const char *szTitle = "Sandbox Platformer: Game Window", const Flags eFlags = Flags::None);

    /*****************************************************
     * ~GameWindow
     *
     * Destroys the Window && Shutsdown BGFX
     *****************************************************/
    ~GameWindow();

    void SetViewMatrix(const glm::mat4 &m4View) {
        this->m_m4View = m4View;

        auto view = glm::value_ptr(m_m4View);
        auto proj = glm::value_ptr(m_m4Projection);

        bgfx::setViewTransform(0, view, proj);
    }

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

public:
    int32_t Width() {
        return m_iWidth;
    }

    int32_t Height() {
        return m_iHeight;
    }

    const glm::mat4 &ProjectionMatrix() const {
        return m_m4Projection;
    }

private:
    SDL_Window *m_SDLWindow = nullptr;

    int32_t m_iWidth = 0;
    int32_t m_iHeight = 0;

    bool m_bExit = false;

    int64_t m_iLastTime = 0;

    glm::mat4 m_m4Projection;
    glm::mat4 m_m4View;
};