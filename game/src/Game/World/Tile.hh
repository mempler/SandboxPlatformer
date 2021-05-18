#pragma once

#include "Game/Managers/ItemInfoManager.hh"

#include <Kokoro/Memory/Buffer.hh>
#include <glm/glm.hpp>

struct Tile
{
    Item *pFore = 0;  // pointer because they are already loaded
    Item *pBack = 0;  // no need to create them again, check iteminfoman

    uint16_t iPosX = 0;
    uint16_t iPosY = 0;

    // Game stuff
    glm::mat4 m4Transform {};

    void UpdateTransform();
    void Draw();

    // Network stuff
    bool Pack( Kokoro::Memory::Buffer &buffer );
    bool Unpack( uint32_t iWorldVersion, Kokoro::Memory::Buffer &buffer );
};