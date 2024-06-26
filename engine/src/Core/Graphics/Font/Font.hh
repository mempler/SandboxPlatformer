#pragma once

#include "Core/Graphics/Texture2D.hh"
#include "Core/Utils/Identifier.hh"

#include "Platform.hh"

#include <Kokoro/Memory/Span.hh>

#include <freetype-gl.h>

constexpr const char *g_szASCII = " !\"#$%&'()*+,-./"
                                  "0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ["
                                  "\\]^_`abcdefghijklmnopqrstuvwxyz{|}~";

class Font
{
  public:
    Font() {};
    ~Font();

    /*****************************************************
     * Load
     *
     * Loads a truetype format from file and creates renderable bitmap font data
     *
     * @param identifier Identifier of the truetype (E.G file://arial.ttf)
     *
     * @return FreeType font handle (Font)
     *****************************************************/
    static void Load( Font *pDest, Identifier const &identifier, size_t sAtlasWidth,
                      size_t sAtlasHeight, float fSizePX,
                      const char *szChars = g_szASCII );

    /*****************************************************
     * Load
     *
     * Loads a truetype format from memory and creates renderable bitmap font
     *data
     *
     * @param identifier Identifier of the truetype (E.G engine://arial.ttf)
     * @param vData Readonly chunk of truetype data.
     *
     * @return FreeType font handle (Font)
     *****************************************************/
    static void Load( Font *pDest, Identifier const &identifier, size_t sAtlasWidth,
                      size_t sAtlasHeight, float fSizePX,
                      Kokoro::Memory::Span<uint8_t> const &vData,
                      const char *szChars = g_szASCII );

    ftgl::texture_glyph_t *GetGlyph( char cChar );
    float GetKerning( char cBefore, char cCurrent );

  public:
    Texture2D *GetTexture() const
    {
        return m_pTexture;
    }

    ftgl::texture_font_t *GetHandle() const
    {
        return m_pHandle;
    }

  private:
    ftgl::texture_atlas_t *m_pAtlas;
    ftgl::texture_font_t *m_pHandle;

    Texture2D *m_pTexture;
};