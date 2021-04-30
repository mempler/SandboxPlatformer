#pragma once

#include "IDebugUtil.hh"

#include <imgui.h>

class IResourceMonitor : public IDebugUtil {
public:
    IResourceMonitor(Engine *pEngine) : IDebugUtil(pEngine) {
    }

    void Draw() override;
};