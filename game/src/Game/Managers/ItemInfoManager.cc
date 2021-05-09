#include "ItemInfoManager.hh"

void ItemInfoManager::Preload(const glm::vec4 &v4UVs, Texture2D *pTexture) {
    Item item{ (uint16_t)m_vItems.size(), v4UVs, pTexture };
    m_vItems.push_back(item);
}

Item *ItemInfoManager::GetItem(uint16_t uID) {
    if (uID > m_vItems.size() + 1)
        return 0;

    return &m_vItems[uID];
}