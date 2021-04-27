#pragma once

#include <string_view>
#include <unordered_map>

#include "Core/Graphics/Texture2D.hh"
#include "Core/Utils/Identifier.hh"

class TextureManager {
public:
    // file://Path/To/File -> ./Path/To/File
    Texture2D const *Load(Identifier const &identifier);
    Texture2D const *Load(const char *szUri) {
        return Load(Identifier(szUri));
    }
    Texture2D const *Load(std::string_view const &svUri) {
        return Load(Identifier(svUri));
    }
    Texture2D const *Load(std::string const &sUri) {
        return Load(Identifier(sUri));
    }

    void DestroyAll();

private:
    std::unordered_map<Identifier, Texture2D> m_umTextures;
};
