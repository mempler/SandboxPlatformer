#pragma once

#include "IDebugUtil.hh"

#include "Core/GUI/GUI.hh"

class GUIInspector : public IDebugUtil {
public:
    GUIInspector(Engine *pEngine) : IDebugUtil(pEngine) {
    }

    void Draw() override;

private:
};
