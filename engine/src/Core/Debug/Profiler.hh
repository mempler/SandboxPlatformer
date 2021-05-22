#pragma once

#include "IDebugUtil.hh"

#include "Core/Utils/Timer.hh"

#include <bgfx/bgfx.h>

#include <array>

class Profiler : public IDebugUtil
{
  public:
    Profiler( Engine *pEngine ) : IDebugUtil( pEngine )
    {
    }

    void Draw() override;

    const char *Name() override
    {
        return "Performance Profiler";
    }

  private:
    Timer timer;

    float m_fHighestFPS = 0;
    std::array<float, 60> m_vFrames {};  // Keep track of 300 draw calls

    bgfx::FrameBufferHandle m_hFrameBuffer = BGFX_INVALID_HANDLE;
};