#pragma once

#include "IResourceManager.hh"

#include "Core/Graphics/Texture2D.hh"
#include "Core/Utils/Identifier.hh"

class TextureLoader : public IResourceLoader<Texture2D> {
public:
    void Load(Texture2D *pDest, const Identifier &identifier) override {
        Texture2D::Load(pDest, identifier);
    }
};

class TextureManager : public IResourceManager<Texture2D, TextureLoader> {
public:
    Texture2D *CreateTextureWithColor(
        Identifier const &identifier, int32_t iWidth, int32_t iHeight, bgfx::TextureFormat::Enum eTextureFormat, uint64_t u64Filters, uint32_t uColor) {
        // Return cached texture
        if (Has(identifier))
            return Load(identifier);

        // Otherwise we create it
        eastl::span<uint8_t> whiteTextureDataPtr = eastl::span((uint8_t *)&uColor, 4);

        Texture2D *resource = CreateEmpty(identifier);
        Texture2D::LoadRaw(resource, identifier, iWidth, iHeight, eTextureFormat, u64Filters, whiteTextureDataPtr);

        return resource;
    }
};
