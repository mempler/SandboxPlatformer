#pragma once

#include "Core/Graphics/Texture2D.hh"
#include "Core/Utils/Identifier.hh"

#include <EASTL/unordered_map.h>

class TextureManager {
public:
    // file://Path/To/File -> ./Path/To/File
    Texture2D const *Load(Identifier const &identifier);

    Texture2D const *HandOff(Texture2D &&tex);

    void DestroyAll();

private:
    eastl::unordered_map<Identifier, Texture2D> m_umTextures;
};
