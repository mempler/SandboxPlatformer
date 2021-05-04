#include "pch.hh"

#include "Font.hh"

#include "Core/Engine.hh"

Font::~Font() {
    ftgl::texture_atlas_delete(m_pAtlas);
    ftgl::texture_font_delete(m_pHandle);

    // how to destroy Texture2D?
}

void Font::Load(Font *pDest, Identifier const &identifier, size_t sAtlasWidth, size_t sAtlasHeight, float fSizePX, const char *szChars) {
    if (pDest == nullptr)
        return;

    pDest->m_pAtlas = ftgl::texture_atlas_new(sAtlasWidth, sAtlasHeight, 4);
    pDest->m_pHandle = ftgl::texture_font_new_from_file(pDest->m_pAtlas, fSizePX, identifier.Path().data());
    ftgl::texture_font_load_glyphs(pDest->m_pHandle, szChars);

    pDest->m_pTexture = GetEngine()->GetTextureManager().CreateTextureFromMemory(identifier.Raw() + "-ch-tx-" + std::to_string((int)fSizePX), sAtlasWidth, sAtlasHeight,
        bgfx::TextureFormat::RGBA8, BGFX_SAMPLER_NONE, pDest->m_pAtlas->data,
        pDest->m_pAtlas->width * pDest->m_pAtlas->height * pDest->m_pAtlas->depth);
}

void Font::Load(Font *pDest, Identifier const &identifier, size_t sAtlasWidth, size_t sAtlasHeight, float fSizePX, tcb::span<uint8_t> const &vData, const char *szChars) {
    if (pDest == nullptr)
        return;

    pDest->m_pAtlas = ftgl::texture_atlas_new(sAtlasWidth, sAtlasHeight, 4);
    pDest->m_pHandle = ftgl::texture_font_new_from_memory(pDest->m_pAtlas, fSizePX, vData.data(), vData.size());
    ftgl::texture_font_load_glyphs(pDest->m_pHandle, szChars);

    pDest->m_pTexture = GetEngine()->GetTextureManager().CreateTextureFromMemory(identifier.Raw() + "-ch-tx-" + std::to_string((int)fSizePX), sAtlasWidth, sAtlasHeight,
        bgfx::TextureFormat::RGBA8, BGFX_SAMPLER_NONE, pDest->m_pAtlas->data,
        pDest->m_pAtlas->width * pDest->m_pAtlas->height * pDest->m_pAtlas->depth);
}

ftgl::texture_glyph_t *Font::GetGlyph(char cChar) {
    return ftgl::texture_font_get_glyph(m_pHandle, &cChar);
}

float Font::GetKerning(char cBefore, char cCurrent) {
    return ftgl::texture_glyph_get_kerning(GetGlyph(cCurrent), &cBefore);
}