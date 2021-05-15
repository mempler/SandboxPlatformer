#pragma once

#include "Core/Graphics/Texture2D.hh"

struct Item {
    uint16_t uID = 0;

    glm::vec4 v4UVs{};

    Texture2D *pAtlas = 0;
};

class ItemInfoManager {
public:
    ItemInfoManager(){};
    ~ItemInfoManager(){};

    void Preload(const glm::vec4 &v4UVs, Texture2D *pTexture);

    Item *GetItem(uint16_t uID);

private:
    std::vector<Item> m_vItems;
};