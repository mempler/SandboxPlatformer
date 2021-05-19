#pragma once

#include "Core/Utils/Identifier.hh"

#include "Platform.hh"

#include <Kokoro/Memory/Span.hh>
#include <bgfx/bgfx.h>

#include <stdint.h>

#define TEXTURE_FORMAT_NEAREST ( BGFX_SAMPLER_MIN_POINT | BGFX_SAMPLER_MAG_POINT )

// Low level texture wrapper
class ENGINE_EXPORT Texture2D
{
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
    static void Load( Texture2D *pDest, Identifier const &identifier,
                      uint64_t u64Filters );

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
    static void Load( Texture2D *pDest, Identifier const &identifier, uint64_t u64Filters,
                      Kokoro::Memory::Span<uint8_t> const &vData );

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
    static void LoadRaw( Texture2D *pDest, Identifier const &identifier, int32_t iWidth,
                         int32_t iHeight, bgfx::TextureFormat::Enum eTextureFormat,
                         uint64_t u64Filters,
                         Kokoro::Memory::Span<uint8_t> const &vData );

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
    static void Create( Texture2D *pDest, Identifier const &identifier, int32_t iWidth,
                        int32_t iHeight, bgfx::TextureFormat::Enum eTextureFormat );

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
     * @param vData Readonly chunk of pixel data that will be copied onto this
     *texture.
     *****************************************************/
    void Modify( int32_t iPosX, int32_t iPosY, int32_t iWidth, int32_t iHeight,
                 bgfx::TextureFormat::Enum eTextureFormat,
                 Kokoro::Memory::Span<uint8_t> const &vData );

  public:
    /*****************************************************
     * GetWidth
     *
     * Gets the current Texture Width
     *
     * @return Texture Width
     *****************************************************/
    uint32_t GetWidth() const
    {
        return m_iWidth;
    }

    /*****************************************************
     * GetHeight
     *
     * Gets the current Texture Height
     *
     * @return Texture Height
     *****************************************************/
    uint32_t GetHeight() const
    {
        return m_iHeight;
    }

    /*****************************************************
     * GetDataSize
     *
     * Gets the current Texture data size
     *
     * @return Texture data size
     *****************************************************/
    uint32_t GetDataSize() const
    {
        return m_uDataSize;
    }

    /*****************************************************
     * GetHandle
     *
     * Gets the active texture Handle (for internal use)
     *
     * @return Texture Handle
     *****************************************************/
    bgfx::TextureHandle GetHandle() const
    {
        return m_thHandle;
    }

    /*****************************************************
     * GetHandle
     *
     * Gets the Identifier of this texture
     *
     * @return Texture Handle
     *****************************************************/
    Identifier GetIdentifier() const
    {
        return m_Identifier;
    }

    /*****************************************************
     * IsValid
     *
     * Checks if the current texture is valid
     *
     * @return False if invalid
     *****************************************************/
    bool IsValid()
    {
        return bgfx::isValid( m_thHandle );
    }

  private:
    Identifier m_Identifier = "engine://invalid_texture";

    int32_t m_iWidth = 0;
    int32_t m_iHeight = 0;

    uint32_t m_uDataSize = 0;

    bgfx::TextureHandle m_thHandle = BGFX_INVALID_HANDLE;
};