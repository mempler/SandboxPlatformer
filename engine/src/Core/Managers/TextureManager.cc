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

void TextureManager::DestroyAll() {
    m_umTextures.clear();
}
