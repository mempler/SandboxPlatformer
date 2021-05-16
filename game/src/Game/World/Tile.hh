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

#if !GAME_SERVER
    glm::mat4 m4Transform {};

    void UpdateTransform();

    void RenderForeground();
    void RenderBackground();
    void RenderTileShadow();
#endif

    // Network stuff
    void Pack( Kokoro::Memory::Buffer &buffer );
    void Unpack( uint32_t iWorldVersion, Kokoro::Memory::Buffer &buffer );
};