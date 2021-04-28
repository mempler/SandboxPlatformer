#include "Texture2D.hh"

#include "Core/Utils/FileSystem.hh"

namespace bgfx {
    // Hack for getting BGFX Allocator
    extern bx::AllocatorI *g_allocator;
} // namespace bgfx

static void DeleteImageContainer(void *vpPtr, void *vpUserData) {
    BX_UNUSED(vpPtr)
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
Texture2D Texture2D::Load(std::string_view svPath) {
    std::vector<uint8_t> data = FileSystem::ReadBinaryFile(svPath); // what the hell
    return Texture2D::Load(FileSystem::GetFileName(svPath), data.data(), data.size());
}

/*****************************************************
 * Load
 *
 * Loads a texture from Memory into GPU Memory
 *
 * @return GPU Texture wrapper (Texture2D)
 *****************************************************/
Texture2D Texture2D::Load(std::string_view svName, uint8_t *pMem, uint32_t uMemSize) {
    LOG_INFO("Loading Texture2D <%s>", svName.data());

    // Parse the input image
    auto *imageContainer = bimg::imageParse(bgfx::g_allocator, pMem, uMemSize);

    if (imageContainer == nullptr)
        return {}; // Return invalid texture

    const auto *const pixelData = bgfx::makeRef(imageContainer->m_data, imageContainer->m_size, DeleteImageContainer, imageContainer);

    // Make sure we have a valid texture
    if (!bgfx::isTextureValid(0, false, imageContainer->m_numLayers, (bgfx::TextureFormat::Enum)imageContainer->m_format, bgfx::TextureFormat::RGBA4))
        return {};

    Texture2D texture;
    texture.m_thHandle = bgfx::createTexture2D((uint16_t)imageContainer->m_width, (uint16_t)imageContainer->m_height, 1 < imageContainer->m_numMips,
        imageContainer->m_numLayers, (bgfx::TextureFormat::Enum)imageContainer->m_format, bgfx::TextureFormat::RGBA4, pixelData);

    texture.m_iWidth = imageContainer->m_width;
    texture.m_iHeight = imageContainer->m_height;

    if (!texture.IsValid())
        return texture; // This wont really matter tbh

    bgfx::setName(texture.m_thHandle, svName.data(), svName.length());

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
    std::string_view svName, int32_t iWidth, int32_t iHeight, bgfx::TextureFormat::Enum eTextureFormat, uint64_t u64Filters, uint8_t *pMem, uint32_t uMemSize) {
    LOG_INFO("Loading Raw Texture2D <%s>(%d, %d)", svName.data(), iWidth, iHeight);
    const auto *const pixelData = bgfx::copy(pMem, uMemSize); // dont use makeRef dont use makeRef dont use makeRef dont use makeRef

    Texture2D texture;
    texture.m_thHandle = bgfx::createTexture2D((uint16_t)iWidth, (uint16_t)iHeight, false, 1, eTextureFormat, u64Filters, pixelData);

    texture.m_iWidth = iWidth;
    texture.m_iHeight = iHeight;

    if (!texture.IsValid())
        return texture; // Doesn't really matter

    bgfx::setName(texture.m_thHandle, svName.data(), (int32_t)svName.length());

    return texture;
}

/*****************************************************
 * Create
 *
 * Creates a Texture2D on the GPU
 *
 * @return GPU Texture wrapper (Texture2D)
 *****************************************************/
Texture2D Texture2D::Create(std::string_view svName, int32_t iWidth, int32_t iHeight, bgfx::TextureFormat::Enum eTextureFormat) {
    Texture2D texture;
    texture.m_thHandle = bgfx::createTexture2D(iWidth, iHeight, false, 1, eTextureFormat, eTextureFormat);

    if (!texture.IsValid())
        return texture; // Doesn't really matter lol

    texture.m_iWidth = iWidth;
    texture.m_iHeight = iHeight;

    bgfx::setName(texture.m_thHandle, svName.data(), (uint32_t)svName.length());

    return texture;
}

/*****************************************************
 * Modify
 *
 * Modifies a texture at Position
 *
 *****************************************************/
void Texture2D::Modify(int32_t iPosX, int32_t iPosY, int32_t iWidth, int32_t iHeight, bgfx::TextureFormat::Enum eTextureFormat, uint8_t *pMem, uint32_t uMemSize) {
    if (pMem || iWidth == 0 || iHeight == 0)
        return;

    auto pixelData = bgfx::copy(pMem, uMemSize);

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
