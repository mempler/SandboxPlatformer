#include "pch.hh"

#include "Texture2D.hh"

#include "Core/Utils/FileSystem.hh"
#include "Core/Utils/Identifier.hh"

namespace bgfx {
    // Hack for getting BGFX Allocator
    extern bx::AllocatorI *g_allocator;
} // namespace bgfx

static void DeleteImageContainer(void *vpPtr, void *vpUserData) {
    EA_UNUSED(vpPtr);
    bimg::ImageContainer *const imageContainer = (bimg::ImageContainer *)vpUserData;
    bimg::imageFree(imageContainer);
}

Texture2D ::~Texture2D() {
    LOG_INFO("Destroying Texture");

    // Destroy GPU Texture Handle
    if (bgfx::isValid(this->m_thHandle))
        bgfx::destroy(this->m_thHandle);
}

/*****************************************************
 * Load
 *
 * Loads a texture from FS into GPU Memory
 *
 * @return GPU Texture wrapper (Texture2D)
 *****************************************************/
Texture2D Texture2D::Load(Identifier const &identifier) {
    std::vector<uint8_t> data;
    if (identifier.Protocol() == "file") {
        data = FileSystem::ReadBinaryFile(identifier.Path().data()); // what the hell
    }

    return Texture2D::Load(identifier, data);
}

/*****************************************************
 * Load
 *
 * Loads a texture from Memory into GPU Memory
 *
 * @return GPU Texture wrapper (Texture2D)
 *****************************************************/
Texture2D Texture2D::Load(Identifier const &identifier, eastl::span<uint8_t> const &vData) {
    LOG_INFO("Loading Texture2D <%s>", identifier.Raw().data());

    // Parse the input image
    auto *imageContainer = bimg::imageParse(bgfx::g_allocator, vData.data(), vData.size());

    if (imageContainer == nullptr)
        return {}; // Return invalid texture

    const auto *const pixelData = bgfx::makeRef(imageContainer->m_data, imageContainer->m_size, DeleteImageContainer, imageContainer);

    // Make sure we have a valid texture
    if (!bgfx::isTextureValid(0, false, imageContainer->m_numLayers, (bgfx::TextureFormat::Enum)imageContainer->m_format, bgfx::TextureFormat::RGBA4))
        return {};

    Texture2D texture;
    texture.m_Identifier = identifier;
    texture.m_thHandle = bgfx::createTexture2D((uint16_t)imageContainer->m_width, (uint16_t)imageContainer->m_height, 1 < imageContainer->m_numMips,
        imageContainer->m_numLayers, (bgfx::TextureFormat::Enum)imageContainer->m_format, bgfx::TextureFormat::RGBA4, pixelData);

    texture.m_iWidth = imageContainer->m_width;
    texture.m_iHeight = imageContainer->m_height;

    if (!texture.IsValid())
        return texture; // This wont really matter tbh

    eastl::string bgfxName = identifier.Raw();
    bgfx::setName(texture.m_thHandle, bgfxName.data(), bgfxName.length());

    return texture;
}

/*****************************************************
 * LoadRaw
 *
 * Loads a texture from Memory into GPU Memory
 * 
 * @return GPU Texture wrapper (Texture2D)
 *****************************************************/
Texture2D Texture2D::LoadRaw(
    Identifier const &identifier, int32_t iWidth, int32_t iHeight, bgfx::TextureFormat::Enum eTextureFormat, uint64_t u64Filters, eastl::span<uint8_t> const &vData) {
    LOG_INFO("Loading Raw Texture2D <%s>(%d, %d)", identifier.Raw().data(), iWidth, iHeight);
    const auto *const pixelData = bgfx::copy(vData.data(), vData.size()); // dont use makeRef dont use makeRef dont use makeRef dont use makeRef

    Texture2D texture;
    texture.m_Identifier = identifier;
    texture.m_thHandle = bgfx::createTexture2D((uint16_t)iWidth, (uint16_t)iHeight, false, 1, eTextureFormat, u64Filters, pixelData);

    texture.m_iWidth = iWidth;
    texture.m_iHeight = iHeight;

    if (!texture.IsValid())
        return std::move(texture); // Doesn't really matter

    eastl::string bgfxName = identifier.Raw();
    bgfx::setName(texture.m_thHandle, bgfxName.data(), bgfxName.length());

    return std::move(texture);
}

/*****************************************************
 * Create
 *
 * Creates a Texture2D on the GPU
 *
 * @return GPU Texture wrapper (Texture2D)
 *****************************************************/
Texture2D Texture2D::Create(Identifier const &identifier, int32_t iWidth, int32_t iHeight, bgfx::TextureFormat::Enum eTextureFormat) {
    Texture2D texture;
    texture.m_Identifier = identifier;
    texture.m_thHandle = bgfx::createTexture2D(iWidth, iHeight, false, 1, eTextureFormat, eTextureFormat);

    if (!texture.IsValid())
        return texture; // Doesn't really matter lol

    texture.m_iWidth = iWidth;
    texture.m_iHeight = iHeight;

    eastl::string bgfxName = identifier.Raw();
    bgfx::setName(texture.m_thHandle, bgfxName.data(), (uint32_t)bgfxName.length());

    return texture;
}

/*****************************************************
 * Modify
 *
 * Modifies a texture at Position
 *
 *****************************************************/
void Texture2D::Modify(int32_t iPosX, int32_t iPosY, int32_t iWidth, int32_t iHeight, bgfx::TextureFormat::Enum eTextureFormat, eastl::span<uint8_t> const &vData) {
    if (iWidth == 0 || iHeight == 0 || vData.size() == 0)
        return;

    auto pixelData = bgfx::copy(vData.data(), vData.size());

    // Hack for figuring out our _pitch
    size_t pitch = 0;
    switch (eTextureFormat) {
        case bgfx::TextureFormat::R8: pitch = 1; break;
        case bgfx::TextureFormat::RG8: pitch = 2; break;
        case bgfx::TextureFormat::RGB8: pitch = 3; break;
        case bgfx::TextureFormat::RGBA8: pitch = 4; break;
        default: pitch = 1; break;
    }

    bgfx::updateTexture2D(m_thHandle, 0, 0, iPosX, iPosY, iWidth, iHeight, pixelData, iHeight * pitch);
}
