#pragma once

#include "pch.hh"

#include "Tile.hh"

class World {
public:
    World();
    ~World();

    void Init();

private:
    std::vector<Tile> m_vTiles;
};