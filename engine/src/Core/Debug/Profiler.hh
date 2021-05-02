#pragma once

#include "IDebugUtil.hh"

#include "Core/Utils/Timer.hh"

#include "bgfx/bgfx.h"

#include <EASTL/array.h>

class Profiler : public IDebugUtil {
public:
    Profiler(Engine *pEngine) : IDebugUtil(pEngine) {
    }

    void Draw() override;

private:
    Timer timer;

    float m_fHighestFPS = 0;
    eastl::array<float, 60> m_vFrames{}; // Keep track of 300 draw calls

    bgfx::FrameBufferHandle m_hFrameBuffer = BGFX_INVALID_HANDLE;
};