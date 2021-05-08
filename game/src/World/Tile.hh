#pragma once

#include "pch.hh"

#include "Core/Graphics/Texture2D.hh"

class Tile {
public: // for now, its ok
    uint16_t uForeID = 0;
    uint16_t uBackID = 0;

    glm::mat4 m4Transform{};
    glm::vec4 v4UVs{};

    Texture2D *pAtlas = 0;
};