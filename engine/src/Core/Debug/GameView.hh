#pragma once

#include "IDebugUtil.hh"

#include "bgfx/bgfx.h"

class GameView : public IDebugUtil
{
  public:
    GameView( Engine *pEngine ) : IDebugUtil( pEngine )
    {
    }

    void Draw() override;

    const char *Name() override
    {
        return "Game View";
    }

  private:
    bgfx::FrameBufferHandle m_hFrameBuffer = BGFX_INVALID_HANDLE;
};