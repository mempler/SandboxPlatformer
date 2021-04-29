#include "pch.hh"

#include "TextureManager.hh"

#include "Core/Graphics/Texture2D.hh"
#include "Core/Utils/FileSystem.hh"
#include "Core/Utils/Logger.hh"

Texture2D const *TextureManager::Load(Identifier const &rIdent) {
    if (m_umTextures.find(rIdent) != m_umTextures.end())
        return &m_umTextures.at(rIdent);

    // file://
    if (rIdent.Protocol() == "file") {
        Texture2D texture = Texture2D::Load(rIdent.Path());

        this->m_umTextures.insert_or_assign(rIdent, texture);

        return &m_umTextures.at(rIdent);
    }
    // TODO:
    //   http://
    //   pkg://
    else {
        // TODO: throw an exception
        LOG_WARN("Protocol %s is not supported!", rIdent.Protocol());
    }

    return nullptr;
}

Texture2D const *TextureManager::HandOff(Texture2D &&tex) {
    auto identifier = tex.GetIdentifier();

    // Duplicate texture, lets remove the new one.
    if (m_umTextures.find(identifier) != m_umTextures.end())
        return &m_umTextures.at(identifier);

    // Otherwise we just add it
    this->m_umTextures[identifier] = tex;

    LOG_INFO("Not yet destroyed");

    return &m_umTextures.at(identifier);
}

void TextureManager::DestroyAll() {
    m_umTextures.clear();
}
