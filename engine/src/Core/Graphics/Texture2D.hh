#pragma once

#include "Core/Utils/Identifier.hh"

#include "Platform.hh"

#include <EASTL/span.h>
#include <EASTL/string_view.h>
#include <bgfx/bgfx.h>

// Low level texture wrapper
class ENGINE_EXPORT Texture2D {
public:
    ~Texture2D();

    /*****************************************************
     * Load
     *
     * Loads a texture from FS into GPU Memory
     *
     * @param identifier Identifier of the texture (E.G file://image.png)
     *
     * @return GPU Texture wrapper (Texture2D)
     *****************************************************/
    static Texture2D Load(Identifier const &identifier);

    /*****************************************************
     * Load
     *
     * Loads a texture from Memory into GPU Memory
     *
     * @param identifier Identifier of the texture (E.G engine://image.png)
     * @param vData Readonly chunk of pixel data.
     *
     * @return GPU Texture wrapper (Texture2D)
     *****************************************************/
    static Texture2D Load(Identifier const &identifier, eastl::span<uint8_t> const &vData);

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
     * @param vData Readonly chunk of pixel data.
     *
     * @return GPU Texture wrapper (Texture2D)
     *****************************************************/
    static Texture2D LoadRaw(
        Identifier const &identifier, int32_t iWidth, int32_t iHeight, bgfx::TextureFormat::Enum eTextureFormat, uint64_t u64Filters, eastl::span<uint8_t> const &vData);

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
    static Texture2D Create(Identifier const &identifier, int32_t iWidth, int32_t iHeight, bgfx::TextureFormat::Enum eTextureFormat);

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
     * @param vData Readonly chunk of pixel data that will be copied onto this texture.
     *****************************************************/
    void Modify(int32_t iPosX, int32_t iPosY, int32_t iWidth, int32_t iHeight, bgfx::TextureFormat::Enum eTextureFormat, eastl::span<uint8_t> const &vData);

public:
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
     * GetHandle
     *
     * Gets the Identifier of this texture
     *
     * @return Texture Handle
     *****************************************************/
    Identifier GetIdentifier() const {
        return m_Identifier;
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
    Identifier m_Identifier = "engine://invalid_texture";

    int32_t m_iWidth = 0;
    int32_t m_iHeight = 0;

    bgfx::TextureHandle m_thHandle = BGFX_INVALID_HANDLE;
};
