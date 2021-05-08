#include "pch.hh"

#include "Window.hh"

#include "Core/Engine.hh"

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

    m_SDLWindow = SDL_CreateWindow(szTitle, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, iWidth, iHeight, flags | SDL_WINDOW_RESIZABLE);
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
    bgfx::setViewClear(0, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x0000000FF, 1.0f, 0);

    bgfx::setViewRect(0, 0, 0, bgfx::BackbufferRatio::Equal);

#if PLATFORM_ANDROID
    // Set display size
    SDL_DisplayMode displayMode;
    SDL_GetDesktopDisplayMode(0, &displayMode);

    m_iWidth = static_cast<uint32_t>(displayMode.w);
    m_iHeight = static_cast<uint32_t>(displayMode.h);

    bgfx::reset(m_iWidth, m_iHeight, BGFX_RESET_MSAA_X16);
    bgfx::setViewRect(0, 0, 0, bgfx::BackbufferRatio::Equal);
#endif

#if ENGINE_DEBUG
    ImGui::CreateContext();
    ImPlot::CreateContext();

    ImGuiIO &io = ImGui::GetIO();
    (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;   // Enable Docking
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable; // Enable Multi-Viewport / Platform Windows

    ImGui::StyleColorsDark();

    // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
    ImGuiStyle &style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    ImGui_Implbgfx_Init(999);
#if PLATFORM_WIN32
    ImGui_ImplSDL2_InitForD3D(m_SDLWindow);
#elif PLATFORM_LINUX
    ImGui_ImplSDL2_InitForOpenGL(m_SDLWindow, nullptr);
#endif
#endif
}

/*****************************************************
 * ~GameWindow
 *
 * Destroys the Window && Shutsdown BGFX
 *****************************************************/
GameWindow::~GameWindow() {
#if ENGINE_DEBUG
    ImGui_ImplSDL2_Shutdown();
    ImGui_Implbgfx_Shutdown();
#endif

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
#if ENGINE_DEBUG
        if (ImGui_ImplSDL2_ProcessEvent(&e))
            continue;
#endif
        OnSDL2Event(this, e);

        switch (e.type) {
            // Tell our window that we shall exit now
            case SDL_QUIT: m_bExit = true; break;
            case SDL_WINDOWEVENT:
                if (e.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
                    LOG_INFO("Window size changed...");

                    m_iWidth = e.window.data1;
                    m_iHeight = e.window.data2;

                    OnResize(this, m_iWidth, m_iHeight);

                    bgfx::reset(m_iWidth, m_iHeight, BGFX_RESET_NONE);
                    bgfx::setViewRect(0, 0, 0, bgfx::BackbufferRatio::Equal);

                    GetEngine()->GetCamera().SetUniformTransform(0);
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

#if ENGINE_DEBUG
    ImGui_Implbgfx_NewFrame();
    ImGui_ImplSDL2_NewFrame(m_SDLWindow);
    ImGui::NewFrame();
#endif

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
#if ENGINE_DEBUG
    ImGui::Render();
    ImGui_Implbgfx_RenderDrawLists(ImGui::GetDrawData());
#endif

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