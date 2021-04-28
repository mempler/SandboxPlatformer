#pragma once

#include <string_view>

#include "Core/Graphics/Texture2D.hh"
#include "Core/Utils/Identifier.hh"

#include <EASTL/unordered_map.h>

class TextureManager {
public:
    // file://Path/To/File -> ./Path/To/File
    Texture2D const *Load(Identifier const &identifier);

    void DestroyAll();

private:
    eastl::unordered_map<Identifier, Texture2D> m_umTextures;
};
