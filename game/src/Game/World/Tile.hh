#pragma once

#include "Game/Managers/ItemInfoManager.hh"

struct Tile {
    Item *pFore = 0; // pointer because they are already loaded
    Item *pBack = 0; // no need to create them again, check iteminfoman

    glm::vec3 v3Pos{};
    glm::vec2 v2Size{};

    glm::mat4 m4Transform{};

    void UpdateTransform();
    
    void RenderForeground();
    void RenderBackground();
    void RenderTileShadow();
};