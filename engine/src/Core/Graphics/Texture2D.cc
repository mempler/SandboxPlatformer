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
void Texture2D::Load(Texture2D *pDest, Identifier const &identifier) {
    if (pDest == nullptr)
        return;

    std::vector<uint8_t> data;
    if (identifier.Protocol() == "file") {
        data = FileSystem::ReadBinaryFile(identifier.Path().data()); // what the hell
    }

    Texture2D::Load(pDest, identifier, data);
}

/*****************************************************
 * Load
 *
 * Loads a texture from Memory into GPU Memory
 *
 * @return GPU Texture wrapper (Texture2D)
 *****************************************************/
void Texture2D::Load(Texture2D *pDest, Identifier const &identifier, eastl::span<uint8_t> const &vData) {
    if (pDest == nullptr)
        return;

    LOG_INFO("Loading Texture2D <%s>", identifier.Raw().data());

    // Parse the input image
    auto *imageContainer = bimg::imageParse(bgfx::g_allocator, vData.data(), vData.size());

    if (imageContainer == nullptr)
        return;

    const auto *const pixelData = bgfx::makeRef(imageContainer->m_data, imageContainer->m_size, DeleteImageContainer, imageContainer);

    // Make sure we have a valid texture
    if (!bgfx::isTextureValid(0, false, imageContainer->m_numLayers, (bgfx::TextureFormat::Enum)imageContainer->m_format, bgfx::TextureFormat::RGBA4))
        return;

    pDest->m_Identifier = identifier;
    pDest->m_thHandle = bgfx::createTexture2D((uint16_t)imageContainer->m_width, (uint16_t)imageContainer->m_height, 1 < imageContainer->m_numMips,
        imageContainer->m_numLayers, (bgfx::TextureFormat::Enum)imageContainer->m_format, bgfx::TextureFormat::RGBA4, pixelData);

    pDest->m_iWidth = imageContainer->m_width;
    pDest->m_iHeight = imageContainer->m_height;

    if (!pDest->IsValid())
        return;

    eastl::string bgfxName = identifier.Raw();
    bgfx::setName(pDest->m_thHandle, bgfxName.data(), bgfxName.length());
}

/*****************************************************
 * LoadRaw
 *
 * Loads a texture from Memory into GPU Memory
 * 
 * @return GPU Texture wrapper (Texture2D)
 *****************************************************/
void Texture2D::LoadRaw(Texture2D *pDest, Identifier const &identifier, int32_t iWidth, int32_t iHeight, bgfx::TextureFormat::Enum eTextureFormat, uint64_t u64Filters,
    eastl::span<uint8_t> const &vData) {
    LOG_INFO("Loading Raw Texture2D <%s>(%d, %d)", identifier.Raw().data(), iWidth, iHeight);
    const auto *const pixelData = bgfx::copy(vData.data(), vData.size()); // dont use makeRef dont use makeRef dont use makeRef dont use makeRef

    pDest->m_Identifier = identifier;
    pDest->m_thHandle = bgfx::createTexture2D((uint16_t)iWidth, (uint16_t)iHeight, false, 1, eTextureFormat, u64Filters, pixelData);

    pDest->m_iWidth = iWidth;
    pDest->m_iHeight = iHeight;

    if (!pDest->IsValid())
        return;

    eastl::string bgfxName = identifier.Raw();
    bgfx::setName(pDest->m_thHandle, bgfxName.data(), bgfxName.length());
}

/*****************************************************
 * Create
 *
 * Creates a Texture2D on the GPU
 *
 * @return GPU Texture wrapper (Texture2D)
 *****************************************************/
void Texture2D::Create(Texture2D *pDest, Identifier const &identifier, int32_t iWidth, int32_t iHeight, bgfx::TextureFormat::Enum eTextureFormat) {
    pDest->m_Identifier = identifier;
    pDest->m_thHandle = bgfx::createTexture2D(iWidth, iHeight, false, 1, eTextureFormat, eTextureFormat);

    if (!pDest->IsValid())
        return;

    pDest->m_iWidth = iWidth;
    pDest->m_iHeight = iHeight;

    eastl::string bgfxName = identifier.Raw();
    bgfx::setName(pDest->m_thHandle, bgfxName.data(), (uint32_t)bgfxName.length());
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
