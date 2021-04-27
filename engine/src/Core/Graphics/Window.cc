#include "Window.hh"

GameWindow::GameWindow(const int32_t iWidth, const int32_t iHeight, const char *szTitle, const Flags eFlags) {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) < 0) {
        LOG_ERROR("Failed to initialize SDL2!");
        return;
    }

#if PLATFORM_ANDROID
    SDL_SetHint(SDL_HINT_ORIENTATIONS, "LandscapeLeft LandscapeRight");
    m_SDLWindow = SDL_CreateWindow(szTitle, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 0, 0, 0);
#else
    int32_t flags = 0;
    if ((eFlags & Flags::Fullscreen) > 0)
        flags = SDL_WINDOW_FULLSCREEN;

    m_SDLWindow = SDL_CreateWindow(szTitle, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, iWidth, iHeight, flags);
#endif

    if (m_SDLWindow == nullptr) {
        LOG_ERROR("Failed to create window!");
        return;
    }

    // create platform data for BGFX
    bgfx::PlatformData platformData;
    SDL_SysWMinfo wmInfo;
    SDL_VERSION(&wmInfo.version)
    SDL_GetWindowWMInfo(m_SDLWindow, &wmInfo);

#if PLATFORM_WIN32
    platformData.nwh = wmInfo.info.win.window;
#elif PLATFORM_LINUX
    platformData.ndt = wmInfo.info.x11.display;
    platformData.nwh = (void *)(uintptr_t)wmInfo.info.x11.window;
#elif PLATFORM_EMSCRIPTEN
    platformData.nwh = (void *)"#canvas";
#elif PLATFORM_ANDROID
    platformData.ndt = 0;
    platformData.nwh = (void *)wmInfo.info.android.window;

    // Delete existing surface
    // TODO: use EGL for Wayland on Linux
    eglDestroySurface(eglGetDisplay(EGL_DEFAULT_DISPLAY), wmInfo.info.android.surface);
#else
#error "Platform not implemented!"
#endif

    // We need those variables internally
    m_iWidth = iWidth;
    m_iHeight = iHeight;

    // Initialize BGFX using our Platform data
    bgfx::Init bgfxInit;
    // bgfxInit.debug = true; // This slows down the renderer quite a bit

    bgfxInit.type = bgfx::RendererType::Count; // Automatically choose a renderer. OpenGL for RENDERDOC.
    bgfxInit.platformData = platformData;
    bgfxInit.resolution.width = m_iWidth;
    bgfxInit.resolution.height = m_iHeight;
    bgfxInit.resolution.reset = BGFX_RESET_NONE;
    if (!bgfx::init(bgfxInit)) {
        LOG_ERROR("Failed to initialize BGFX!");
        return;
    }

    LOG_INFO("Game Window Initialized...");
    LOG_INFO("BGFX Initialized...");

    // Setup our drawing surface
    bgfx::setViewClear(0, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x00000000, 1.0f, 0);
    // alpha must be 0, we will apply framebuffers onto it ------------^^

    bgfx::setViewRect(0, 0, 0, bgfx::BackbufferRatio::Equal);

#if PLATFORM_ANDROID
    // Set display size
    SDL_DisplayMode displayMode;
    SDL_GetDesktopDisplayMode(0, &displayMode);

    m_iWidth = static_cast<uint32_t>(displayMode.w);
    m_iHeight = static_cast<uint32_t>(displayMode.h);

    bgfx::reset(m_iWidth, m_iHeight, BGFX_RESET_NONE);
    bgfx::setViewRect(0, 0, 0, bgfx::BackbufferRatio::Equal);
#endif

    // Initial Projection
    m_m4View = glm::mat4(1.0f);
    m_m4Projection = glm::ortho(0.f, (float)m_iWidth, (float)m_iHeight, 0.f, 0.1f, 1000.f);
    m_m4Projection[3].z = 1.f;

    bgfx::setViewTransform(0, glm::value_ptr(m_m4View), glm::value_ptr(m_m4Projection));
}

/*****************************************************
 * ~GameWindow
 *
 * Destroys the Window && Shutsdown BGFX
 *****************************************************/
GameWindow::~GameWindow() {
    // Shutdown BGFX
    bgfx::shutdown();

    // Shutdown SDL2
    SDL_DestroyWindow(m_SDLWindow);
    SDL_Quit();
}

/*****************************************************
 * BeginFrame
 *
 * Gets the current state and begins a new frame.
 *
 * NOTE: This must be called on the Render thread.
 *****************************************************/
double GameWindow::BeginFrame() {
    SDL_Event e;
    while (SDL_PollEvent(&e) != 0) { // TODO: Event Pipeline
        switch (e.type) {
            // Tell our window that we shall exit now
            case SDL_QUIT: m_bExit = true; break;
            case SDL_WINDOWEVENT:
                if (e.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
                    LOG_INFO("Window size changed...");

                    SDL_DisplayMode displayMode;
                    SDL_GetDesktopDisplayMode(0, &displayMode);

                    m_iWidth = displayMode.w;
                    m_iHeight = displayMode.h;

                    // Update Projection Matrix
                    m_m4Projection = glm::ortho(0.f, (float)m_iWidth, (float)m_iHeight, 0.f, 0.1f, 1000.f);
                    m_m4Projection[3].z = 1.f;

                    bgfx::reset(m_iWidth, m_iHeight, BGFX_RESET_NONE);
                    bgfx::setViewRect(0, 0, 0, bgfx::BackbufferRatio::Equal);

                    bgfx::setViewTransform(0, glm::value_ptr(m_m4View), glm::value_ptr(m_m4Projection));
                }
                break;
            default: break;
        }
    }

    const auto now = bx::getHPCounter();
    const auto frameTime = now - m_iLastTime;
    m_iLastTime = now;

    const auto freq = static_cast<float>(bx::getHPFrequency());
    const auto delta = static_cast<float>(frameTime) / freq;

    // This dummy draw call is here to make sure that view 0 is cleared if no
    // other draw calls are submitted to view 0.
    bgfx::touch(0);

    return delta;
}

/*****************************************************
 * EndRender
 *
 * Ends the current frame and flushes it to the Window
 *
 * NOTE: This must be called on the Render thread.
 *****************************************************/
void GameWindow::EndFrame() {
    bgfx::frame();
}

/*****************************************************
 * ShouldExit
 *
 * Checks if this window should exit
 * cause E.G Exit() was called
 *
 * @return true if exit
 *****************************************************/
bool GameWindow::ShouldExit() {
    return m_bExit;
}