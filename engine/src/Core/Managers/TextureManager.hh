#pragma once

#include "IResourceManager.hh"

#include "Core/Graphics/Texture2D.hh"
#include "Core/Utils/Identifier.hh"

#include <glm/packing.hpp>

class TextureLoader : public IResourceLoader<Texture2D> {
public:
    void Load(Texture2D *pDest, const Identifier &identifier) override {
        ZoneScoped;
        Texture2D::Load(pDest, identifier, 0);
    }
};

class TextureManager : public IResourceManager<Texture2D, TextureLoader> {
public:
    Texture2D *CreateTextureWithColor(int32_t iWidth, int32_t iHeight, uint64_t u64Filters, glm::vec4 v4Color) {
        ZoneScoped;
        uint32_t data = glm::packUnorm4x8(v4Color);
        Identifier identifier = fmt::format("color://{:x}", data); // Return cached texture
        if (Has(identifier))
            return Load(identifier);

        // Otherwise we create it
        tcb::span<uint8_t> textureDataPtr = tcb::span((uint8_t *)&data, 4);

        Texture2D *resource = CreateEmpty(identifier);
        Texture2D::LoadRaw(resource, identifier, iWidth, iHeight, bgfx::TextureFormat::RGBA8, u64Filters, textureDataPtr);

        return resource;
    }

    Texture2D *CreateTextureFromMemory(Identifier const &identifier, int32_t iWidth, int32_t iHeight, bgfx::TextureFormat::Enum eTextureFormat, uint64_t u64Filters,
        uint8_t *pData, uint32_t uDataSize) {
        ZoneScoped;
        // Return cached texture
        if (Has(identifier))
            return Load(identifier);

        // Otherwise we create it
        tcb::span<uint8_t> textureDataPtr = tcb::span(pData, uDataSize);

        Texture2D *resource = CreateEmpty(identifier);
        Texture2D::LoadRaw(resource, identifier, iWidth, iHeight, eTextureFormat, u64Filters, textureDataPtr);

        return resource;
    }

    Texture2D *CreateTextureFromFile(Identifier const &identifier, uint64_t u64Filters) {
        ZoneScoped;
        // Return cached texture
        if (Has(identifier))
            return Load(identifier);

        Texture2D *resource = CreateEmpty(identifier);
        Texture2D::Load(resource, identifier, u64Filters);

        return resource;
    }
};
