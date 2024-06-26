#pragma once

#include "IResourceManager.hh"

#include "Core/Graphics/Font/Font.hh"
#include "Core/Utils/Identifier.hh"

class FontLoader : public IResourceLoader<Font>
{
  public:
    void Load( Font *pDest, const Identifier &identifier ) override
    {
        ZoneScoped;

        Font::Load( pDest, identifier, 512, 512, 32.f, g_szASCII );
    }
};

class FontManager : public IResourceManager<Font, FontLoader>
{
  public:
    Font *LoadFromMemory( Identifier const &identifier, size_t sAtlasWidth,
                          size_t sAtlasHeight, float fSizePX, uint8_t *pData,
                          uint32_t uDataSize, const char *szChars = g_szASCII )
    {
        ZoneScoped;

        // Return cached texture
        if ( Has( identifier ) ) return Load( identifier );

        // Otherwise we create it
        Kokoro::Memory::Span<uint8_t> dataPtr = Kokoro::Memory::Span( pData, uDataSize );

        Font *resource = new Font;
        Font::Load( resource, identifier, sAtlasWidth, sAtlasHeight, fSizePX, dataPtr,
                    szChars );

        return resource;
    }

    Font *LoadFromFile( Identifier const &identifier, size_t sAtlasWidth,
                        size_t sAtlasHeight, float fSizePX,
                        const char *szChars = g_szASCII )
    {
        ZoneScoped;

        // Return cached texture
        if ( Has( identifier ) ) return Load( identifier );

        Font *resource = new Font;
        Font::Load( resource, identifier, sAtlasWidth, sAtlasHeight, fSizePX, szChars );

        return resource;
    }
};
