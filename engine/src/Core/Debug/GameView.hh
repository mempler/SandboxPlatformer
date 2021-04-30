#pragma once

#include "IDebugUtil.hh"

#include "bgfx/bgfx.h"

class GameView : public IDebugUtil {
public:
    GameView(Engine *pEngine) : IDebugUtil(pEngine) {
    }

    void Draw() override;

private:
    bgfx::FrameBufferHandle m_hFrameBuffer = BGFX_INVALID_HANDLE;
};