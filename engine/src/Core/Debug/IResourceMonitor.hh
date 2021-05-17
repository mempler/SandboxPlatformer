#pragma once

#include "IDebugUtil.hh"

#include <imgui.h>

class IResourceMonitor : public IDebugUtil
{
  public:
    IResourceMonitor( Engine *pEngine ) : IDebugUtil( pEngine )
    {
    }

    const char *Name() override
    {
        return "IResource Monitor";
    }

    void Draw() override;
};