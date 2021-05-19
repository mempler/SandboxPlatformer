#include "pch.hh"

#include "Font.hh"

#include "Core/Engine.hh"
#include "Core/Utils/Identifier.hh"

Font::~Font()
{
    ZoneScoped;

    ftgl::texture_atlas_delete( m_pAtlas );
    ftgl::texture_font_delete( m_pHandle );

    // how to destroy Texture2D?
}

void Font::Load( Font *pDest, Identifier const &identifier, size_t sAtlasWidth,
                 size_t sAtlasHeight, float fSizePX, const char *szChars )
{
    ZoneScoped;

    if ( pDest == nullptr ) return;

    pDest->m_pAtlas = ftgl::texture_atlas_new( sAtlasWidth, sAtlasHeight, 4 );
    pDest->m_pHandle = ftgl::texture_font_new_from_file( pDest->m_pAtlas, fSizePX,
                                                         identifier.Path().data() );
    ftgl::texture_font_load_glyphs( pDest->m_pHandle, szChars );

    std::string proto = fmt::format( "engine://atlastexture:{}:{}px",
                                     identifier.Path().data(), (int) fSizePX );

    pDest->m_pTexture = GetEngine()->GetTextureManager().CreateTextureFromMemory(
        proto, sAtlasWidth, sAtlasHeight, bgfx::TextureFormat::RGBA8, BGFX_SAMPLER_NONE,
        pDest->m_pAtlas->data,
        pDest->m_pAtlas->width * pDest->m_pAtlas->height * pDest->m_pAtlas->depth );
}

void Font::Load( Font *pDest, Identifier const &identifier, size_t sAtlasWidth,
                 size_t sAtlasHeight, float fSizePX,
                 Kokoro::Memory::Span<uint8_t> const &vData, const char *szChars )
{
    ZoneScoped;

    if ( pDest == nullptr ) return;

    pDest->m_pAtlas = ftgl::texture_atlas_new( sAtlasWidth, sAtlasHeight, 4 );
    pDest->m_pHandle = ftgl::texture_font_new_from_memory( pDest->m_pAtlas, fSizePX,
                                                           vData.data(), vData.size() );
    ftgl::texture_font_load_glyphs( pDest->m_pHandle, szChars );

    std::string proto = fmt::format( "engine://atlastexture:{}:{}px",
                                     identifier.Path().data(), (int) fSizePX );

    pDest->m_pTexture = GetEngine()->GetTextureManager().CreateTextureFromMemory(
        proto, sAtlasWidth, sAtlasHeight, bgfx::TextureFormat::RGBA8, BGFX_SAMPLER_NONE,
        pDest->m_pAtlas->data,
        pDest->m_pAtlas->width * pDest->m_pAtlas->height * pDest->m_pAtlas->depth );
}

ftgl::texture_glyph_t *Font::GetGlyph( char cChar )
{
    ZoneScoped;

    return ftgl::texture_font_get_glyph( m_pHandle, &cChar );
}

float Font::GetKerning( char cBefore, char cCurrent )
{
    ZoneScoped;

    return ftgl::texture_glyph_get_kerning( GetGlyph( cCurrent ), &cBefore );
}