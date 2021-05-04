#pragma once

#include "IResourceManager.hh"

#include "Core/Graphics/Texture2D.hh"
#include "Core/Utils/Identifier.hh"

#include "glm/packing.hpp"

class TextureLoader : public IResourceLoader<Texture2D> {
public:
    void Load(Texture2D *pDest, const Identifier &identifier) override {
        Texture2D::Load(pDest, identifier);
    }
};

class TextureManager : public IResourceManager<Texture2D, TextureLoader> {
public:
    Texture2D *CreateTextureWithColor(int32_t iWidth, int32_t iHeight, uint64_t u64Filters, glm::vec4 v4Color) {
        uint32_t data = glm::packUnorm4x8(v4Color);
        Identifier identifier = fmt::format("color://{:x}", data);

        // Return cached texture
        if (Has(identifier))
            return Load(identifier);

        // Otherwise we create it
        tcb::span<uint8_t> textureDataPtr = tcb::span((uint8_t *)&data, 4);

        Texture2D *resource = CreateEmpty(identifier);
        Texture2D::LoadRaw(resource, identifier, iWidth, iHeight, bgfx::TextureFormat::RGBA8, u64Filters, textureDataPtr);

        return resource;
    }
};
