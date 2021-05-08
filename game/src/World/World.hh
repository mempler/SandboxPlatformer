#pragma once

#include "pch.hh"

#include "Tile.hh"

class World {
public:
    World();
    ~World();

private:
    std::vector<Tile> m_vTiles;
};