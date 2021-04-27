#pragma once

#include "pch.hh"
#include <string_view>

#include <string>
#include <vector>

// Low level texture wrapper
class ENGINE_EXPORT Texture2D {
public:
    /*****************************************************
     * Load
     *
     * Loads a texture from FS into GPU Memory
     *
     * @param svPath Path to the Texture
     *
     * @return GPU Texture wrapper (Texture2D)
     *****************************************************/
    static Texture2D Load(std::string_view svPath);

    /*****************************************************
     * Load
     *
     * Loads a texture from Memory into GPU Memory
     *
     * @param svName Name of the texture
     *               (Used for debugging)
     * @param pMem Pixel Data
     * @param uMemSize Memory size
     *
     * @return GPU Texture wrapper (Texture2D)
     *****************************************************/
    static Texture2D Load(std::string_view svName, uint8_t *pMem, uint32_t uMemSize);

    /*****************************************************
     * LoadRaw
     *
     * Loads a texture from Memory into GPU Memory
     *
     * @param svName Name of the texture
     *               (Used for debugging)
     * @param iWidth Texture Width
     * @param iHeight Texture Height
     * @param eTextureFormat Texture Format
     * @param cvData Pixel Data
     *
     * @return GPU Texture wrapper (Texture2D)
     *****************************************************/
    static Texture2D LoadRaw(std::string_view svName, int32_t iWidth, int32_t iHeight, bgfx::TextureFormat::Enum eTextureFormat, uint64_t u64Filters, uint8_t *pMem, uint32_t uMemSize);

    /*****************************************************
     * Create
     *
     * Creates a Texture2D on the GPU
     *
     * @param svName Name of the texture
     *               (Used for debugging)
     * @param iWidth Texture Width
     * @param iHeight Texture Height
     * @param eTextureFormat Texture Format
     *
     * @return GPU Texture wrapper (Texture2D)
     *****************************************************/
    static Texture2D Create(std::string_view svName, int32_t iWidth, int32_t iHeight, bgfx::TextureFormat::Enum eTextureFormat);

    /*****************************************************
     * Modify
     *
     * Modifies a texture at Position
     *
     * @param iPosX Position X
     * @param iPosY Position Y
     * @param iWidth Texture Width
     * @param iHeight Texture Height
     * @param eTextureFormat Texture Format
     * @param pMem Pixel Data
     * @param uMemSize Memory size
     *****************************************************/
    void Modify(int32_t iPosX, int32_t iPosY, int32_t iWidth, int32_t iHeight, bgfx::TextureFormat::Enum eTextureFormat, uint8_t *pMem, uint32_t uMemSize);

public:
    /*****************************************************
     * Destroy
     *
     * Destroys a Texture (unloads from memory)
     *****************************************************/
    void Destroy() {
        LOG_INFO("Destroying Texture");

        // Destroy GPU Texture Handle
        if (bgfx::isValid(this->m_thHandle))
            bgfx::destroy(this->m_thHandle);
    }

    /*****************************************************
     * GetWidth
     *
     * Gets the current Texture Width
     *
     * @return Texture Width
     *****************************************************/
    uint32_t GetWidth() const {
        return m_iWidth;
    }

    /*****************************************************
     * GetHeight
     *
     * Gets the current Texture Height
     *
     * @return Texture Height
     *****************************************************/
    uint32_t GetHeight() const {
        return m_iHeight;
    }

    /*****************************************************
     * GetHandle
     *
     * Gets the active texture Handle (for internal use)
     *
     * @return Texture Handle
     *****************************************************/
    bgfx::TextureHandle GetHandle() const {
        return m_thHandle;
    }

    /*****************************************************
     * IsValid
     *
     * Checks if the current texture is valid
     *
     * @return False if invalid
     *****************************************************/
    bool IsValid() {
        return bgfx::isValid(m_thHandle);
    }

private:
    int32_t m_iWidth = 0;
    int32_t m_iHeight = 0;

    bgfx::TextureHandle m_thHandle = BGFX_INVALID_HANDLE;
};
