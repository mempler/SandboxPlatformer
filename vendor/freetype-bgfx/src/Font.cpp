#include "Font.h"

#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_STROKER_H
// #include FT_ADVANCES_H
#include FT_LCD_FILTER_H
#include "UTF8Utils.h"
#include "Utils.h"
#include "distance-field.h"

#include <assert.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define HRES 64
#define HRESf 64.f
#define DPI 72

namespace ftbgfx {

    static float convert_F26Dot6_to_float(FT_F26Dot6 value) {
        return ((float)value) / 64.0;
    }
    static FT_F26Dot6 convert_float_to_F26Dot6(float value) {
        return (FT_F26Dot6)(value * 64.0);
    }

    // Follows same logic as example provided by documentation:
    // https://www.freetype.org/freetype2/docs/reference/ft2-error_enumerations.html#FTGL_Error_String
    const char *FTGL_Error_String(FT_Error error_code) {
        /* Depending on freetype version header guard macro may vary */
#undef __FTERRORS_H__
#undef FTERRORS_H_

        /* Stream error string table defined in header FT_ERRORS_H to hard-code strings into this lib */
#define FT_ERRORDEF(e, v, s) \
    case e:                  \
        return s;
#define FT_ERROR_START_LIST switch (error_code) {
#define FT_ERROR_END_LIST }
#include FT_ERRORS_H

        /* Fallback if error code was not present in hard coded table (version mismatch between compiled/linked freetype) */
        return "unknown error";

        /* Redefine undefs in case of clashes later on */
#ifndef __FTERRORS_H__
#define __FTERRORS_H__
#endif
#ifndef FTERRORS_H_
#define FTERRORS_H_
#endif
    }

    static int LoadFace(Font *self, float size, FT_Library *library, FT_Face *face) {
        FT_Error error;
        FT_Matrix matrix = { (int)((1.0 / HRES) * 0x10000L), (int)((0.0) * 0x10000L), (int)((0.0) * 0x10000L), (int)((1.0) * 0x10000L) };

        assert(library);
        assert(size);

        /* Initialize library */
        error = FT_Init_FreeType(library);
        if (error) {
            log_error("FT_Error (line %d, 0x%02x) : %s\n", __LINE__, error, FTGL_Error_String(error));
            goto cleanup;
        }

        /* Load face */
        switch (self->m_location) {
            case TEXTURE_FONT_FILE: error = FT_New_Face(*library, self->m_filename, 0, face); break;
            case TEXTURE_FONT_MEMORY: error = FT_New_Memory_Face(*library, self->m_memory.base, self->m_memory.size, 0, face); break;
        }

        if (error) {
            log_error("FT_Error (line %d, code 0x%02x) : %s\n", __LINE__, error, FTGL_Error_String(error));
            goto cleanup_library;
        }

        /* Select charmap */
        error = FT_Select_Charmap(*face, FT_ENCODING_UNICODE);
        if (error) {
            log_error("FT_Error (line %d, code 0x%02x) : %s\n", __LINE__, error, FTGL_Error_String(error));
            goto cleanup_face;
        }

        /* Set char size */
        /* See page 24 of “Higher Quality 2D Text Rendering”:
        * http://jcgt.org/published/0002/01/04/
        * “To render high-quality text, Shemarev [2007] recommends using only
        *  vertical hinting and completely discarding the horizontal hints.
        *  Hinting is the responsibility of the rasterization engine (FreeType in
        *  our case) which provides no option to specifically discard horizontal
        *  hinting. In the case of the FreeType library, we can nonetheless trick
        *  the engine by specifying an oversized horizontal DPI (100 times the
        *  vertical) while specifying a transformation matrix that scale down the
        *  glyph as shown in Listing 1.”
        * That horizontal DPI factor is HRES here. */
        error = FT_Set_Char_Size(*face, convert_float_to_F26Dot6(size), 0, DPI * HRES, DPI);

        if (error) {
            log_error("FT_Error (line %d, code 0x%02x) : %s\n", __LINE__, error, FTGL_Error_String(error));
            goto cleanup_face;
        }

        /* Set transform matrix */
        FT_Set_Transform(*face, &matrix, NULL);

        return 1;

    cleanup_face:
        FT_Done_Face(*face);
    cleanup_library:
        FT_Done_FreeType(*library);
    cleanup:
        return 0;
    }

    static bool InitFont(Font *font) {
        FT_Library library;
        FT_Face face;
        FT_Size_Metrics metrics;

        font->m_height = 0;
        font->m_ascender = 0;
        font->m_rendermode = RENDER_NORMAL;
        font->m_outline_thickness = 0.f;
        font->m_hinting = 1;
        font->m_kerning = 1;
        font->m_filtering = 1;
        font->m_lcd_weights[0] = 0x10;
        font->m_lcd_weights[1] = 0x40;
        font->m_lcd_weights[2] = 0x70;
        font->m_lcd_weights[3] = 0x40;
        font->m_lcd_weights[4] = 0x10;

        if (!LoadFace(font, font->m_size, &library, &face))
            return false;

        font->m_underline_position = face->underline_position / (float)(HRESf * HRESf) * font->m_size;
        font->m_underline_position = roundf(font->m_underline_position);
        if (font->m_underline_position > -2)
            font->m_underline_position = -2.f;

        font->m_underline_thickness = face->underline_thickness / (float)(HRESf * HRESf) * font->m_size;
        font->m_underline_thickness = roundf(font->m_underline_thickness);
        if (font->m_underline_position > 1)
            font->m_underline_position = 1.f;

        metrics = face->size->metrics;
        font->m_ascender = metrics.ascender >> 6;
        font->m_descender = metrics.descender >> 6;
        font->m_height = metrics.height >> 6;
        font->m_linegap = font->m_height - font->m_ascender + font->m_descender;

        FT_Done_Face(face);
        FT_Done_FreeType(library);

        font->GetGlyph(0);

        return true;
    }

    static void GenerateKerning(Font *font, FT_Library *library, FT_Face *face) {
        FT_UInt glyph_index, prev_index;
        Glyph *glyph, *prev_glyph;
        FT_Vector kerning;

        for (size_t i = 1; i < font->m_glyphs.size(); i++) {
            glyph = &font->m_glyphs[i];
            glyph_index = FT_Get_Char_Index(*face, glyph->codepoint);
            glyph->kernings.clear();

            for (size_t j = 0; j < font->m_glyphs.size(); j++) {
                prev_glyph = &font->m_glyphs[j];
                prev_index = FT_Get_Char_Index(*face, prev_glyph->codepoint);
                // FT_KERNING_UNFITTED returns FT_F26Dot6 values.
                FT_Get_Kerning(*face, prev_index, glyph_index, FT_KERNING_UNFITTED, &kerning);

                if (kerning.x)
                    glyph->kernings.push_back({ prev_glyph->codepoint, convert_F26Dot6_to_float(kerning.x) / HRESf });
            }
        }
    }

    Font::Font(TextureAtlas *atlas, float sizePT, const char *path) : m_atlas(atlas), m_size(sizePT), m_location(TEXTURE_FONT_FILE) {
        m_filename = strdup(path);

        if (!InitFont(this)) {
            log_error("Cannot init font: %d\n", __LINE__);
        }
    }

    Font::Font(TextureAtlas *atlas, float sizePT, uint8_t *mem, size_t memSize) : m_atlas(atlas), m_size(sizePT), m_location(TEXTURE_FONT_MEMORY) {
        m_memory.base = mem;
        m_memory.size = memSize;

        if (!InitFont(this)) {
            log_error("Cannot init font: %d\n", __LINE__);
        }
    }

    Font::~Font() {
        Glyph *glyph;

        if (m_location == TEXTURE_FONT_FILE && m_filename)
            free(m_filename);

        m_glyphs.clear();
    }

    Glyph *Font::GetGlyph(const char *codepoint) {
        Glyph *glyph = 0;

        if ((glyph = FindGlyph(codepoint)))
            return glyph;

        if (LoadGlyph(codepoint))
            return FindGlyph(codepoint);

        return 0;
    }

    Glyph *Font::FindGlyph(const char *codepoint) {
        Glyph *glyph = 0;
        uint32_t ucodepoint = utf8_to_utf32(codepoint);

        for (auto &&glyph : m_glyphs)
            if ((glyph.codepoint == ucodepoint) && (ucodepoint == -1) || ((glyph.rendermode == m_rendermode) && (glyph.outline_thickness == m_outline_thickness)))
                return &glyph;

        return 0;
    }

    bool Font::LoadGlyph(const char *codepoint) {
        FT_Library library;
        FT_Error error;
        FT_Face face;
        FT_Glyph ft_glyph;
        FT_GlyphSlot slot;
        FT_Bitmap ft_bitmap;
        FT_UInt glyph_index;
        FT_Int32 flags = 0;
        int ft_glyph_top = 0;
        int ft_glyph_left = 0;
        ivec4 region;
        size_t missed = 0;

        if (!LoadFace(this, m_size, &library, &face))
            return false;

        if (FindGlyph(codepoint)) {
            FT_Done_Face(face);
            FT_Done_FreeType(library);
            return true;
        }

        if (!codepoint) {
            ivec4 region = m_atlas->GetRegion(5, 5);
            Glyph glyph{};
            static unsigned char data[4 * 4 * 3] = { -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
                -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 };

            if (region.x < 0) {
                log_error("Texture atlas is full (line %d)\n", __LINE__);
                FT_Done_Face(face);
                FT_Done_FreeType(library);
                return false;
            }

            m_atlas->SetRegion(region.x, region.y, 4, 4, data, 0);
            glyph.codepoint = -1;
            glyph.s0 = (region.x + 2) / (float)m_atlas->m_width;
            glyph.t0 = (region.y + 2) / (float)m_atlas->m_height;
            glyph.s1 = (region.x + 3) / (float)m_atlas->m_width;
            glyph.t1 = (region.y + 3) / (float)m_atlas->m_height;

            m_glyphs.push_back(glyph);
            FT_Done_Face(face);
            FT_Done_FreeType(library);
            return true;
        }

        flags = 0;
        ft_glyph_top = 0;
        ft_glyph_left = 0;
        glyph_index = FT_Get_Char_Index(face, (FT_ULong)utf8_to_utf32(codepoint));

        if (m_rendermode != RENDER_NORMAL && m_rendermode != RENDER_SIGNED_DISTANCE_FIELD)
            flags |= FT_LOAD_NO_BITMAP;
        else
            flags |= FT_LOAD_RENDER;

        if (!m_hinting)
            flags |= FT_LOAD_NO_HINTING | FT_LOAD_NO_AUTOHINT;
        else
            flags |= FT_LOAD_FORCE_AUTOHINT;

        if (m_atlas->m_depth == 3) {
            FT_Library_SetLcdFilter(library, FT_LCD_FILTER_LIGHT);
            flags |= FT_LOAD_TARGET_LCD;

            if (m_filtering)
                FT_Library_SetLcdFilterWeights(library, m_lcd_weights);
        } else if (HRES == 1)
            flags |= FT_LOAD_TARGET_LIGHT;

        error = FT_Load_Glyph(face, glyph_index, flags);
        if (error) {
            log_error("FT_Error (line %d, code 0x%02x) : %s\n", __LINE__, error, FTGL_Error_String(error));
            FT_Done_Face(face);
            FT_Done_FreeType(library);
            return false;
        }

        if (m_rendermode == RENDER_NORMAL || m_rendermode == RENDER_SIGNED_DISTANCE_FIELD) {
            slot = face->glyph;
            ft_bitmap = slot->bitmap;
            ft_glyph_top = slot->bitmap_top;
            ft_glyph_left = slot->bitmap_left;
        } else {
            FT_Stroker stroker;
            FT_BitmapGlyph ft_bitmap_glyph;

            error = FT_Stroker_New(library, &stroker);

            if (error) {
                log_error("FT_Error (line %d, 0x%02x) : %s\n", __LINE__, error, FTGL_Error_String(error));
                goto cleanup_stroker;
            }

            FT_Stroker_Set(stroker, (int)(m_outline_thickness * HRES), FT_STROKER_LINECAP_ROUND, FT_STROKER_LINEJOIN_ROUND, 0);

            error = FT_Get_Glyph(face->glyph, &ft_glyph);

            if (error) {
                log_error("FT_Error (line %d, 0x%02x) : %s\n", __LINE__, error, FTGL_Error_String(error));
                goto cleanup_stroker;
            }

            if (m_rendermode == RENDER_OUTLINE_EDGE)
                error = FT_Glyph_Stroke(&ft_glyph, stroker, 1);
            else if (m_rendermode == RENDER_OUTLINE_POSITIVE)
                error = FT_Glyph_StrokeBorder(&ft_glyph, stroker, 0, 1);
            else if (m_rendermode == RENDER_OUTLINE_NEGATIVE)
                error = FT_Glyph_StrokeBorder(&ft_glyph, stroker, 1, 1);

            if (error) {
                log_error("FT_Error (line %d, 0x%02x) : %s\n", __LINE__, error, FTGL_Error_String(error));
                goto cleanup_stroker;
            }

            if (m_atlas->m_depth == 1)
                error = FT_Glyph_To_Bitmap(&ft_glyph, FT_RENDER_MODE_NORMAL, 0, 1);
            else
                error = FT_Glyph_To_Bitmap(&ft_glyph, FT_RENDER_MODE_LCD, 0, 1);

            if (error) {
                log_error("FT_Error (line %d, 0x%02x) : %s\n", __LINE__, error, FTGL_Error_String(error));
                goto cleanup_stroker;
            }

            ft_bitmap_glyph = (FT_BitmapGlyph)ft_glyph;
            ft_bitmap = ft_bitmap_glyph->bitmap;
            ft_glyph_top = ft_bitmap_glyph->top;
            ft_glyph_left = ft_bitmap_glyph->left;

        cleanup_stroker:
            FT_Stroker_Done(stroker);

            if (error) {
                FT_Done_Face(face);
                FT_Done_FreeType(library);
                return 0;
            }
        }

        struct {
            int left;
            int top;
            int right;
            int bottom;
        } padding = { 0, 0, 1, 1 };

        if (m_rendermode == RENDER_SIGNED_DISTANCE_FIELD) {
            padding.top = 1;
            padding.left = 1;
        }

        if (m_padding != 0) {
            padding.top += m_padding;
            padding.left += m_padding;
            padding.right += m_padding;
            padding.bottom += m_padding;
        }

        size_t src_w = ft_bitmap.width / m_atlas->m_depth;
        size_t src_h = ft_bitmap.rows;

        size_t tgt_w = src_w + padding.left + padding.right;
        size_t tgt_h = src_h + padding.top + padding.bottom;

        region = m_atlas->GetRegion(tgt_w, tgt_h);

        if (region.x < 0) {
            log_error("Texture atlas is full (line %d)\n", __LINE__);
            FT_Done_Face(face);
            FT_Done_FreeType(library);
            return 0;
        }

        size_t x = region.x;
        size_t y = region.y;

        uint8_t *buffer = (uint8_t *)malloc(tgt_w * tgt_h * m_atlas->m_depth);

        uint8_t *dst_ptr = buffer + (padding.top * tgt_w + padding.left) * m_atlas->m_depth;
        uint8_t *src_ptr = ft_bitmap.buffer;
        for (size_t i = 0; i < src_h; i++) {
            //difference between width and pitch: https://www.freetype.org/freetype2/docs/reference/ft2-basic_types.html#FT_Bitmap
            memcpy(dst_ptr, src_ptr, ft_bitmap.width);
            dst_ptr += tgt_w * m_atlas->m_depth;
            src_ptr += ft_bitmap.pitch;
        }

        if (m_rendermode == RENDER_SIGNED_DISTANCE_FIELD) {
            uint8_t *sdf = make_distance_mapb(buffer, tgt_w, tgt_h);
            free(buffer);
            buffer = sdf;
        }

        m_atlas->SetRegion(x, y, tgt_w, tgt_h, buffer, tgt_w * m_atlas->m_depth);

        free(buffer);

        Glyph glyph{};
        glyph.codepoint = utf8_to_utf32(codepoint);
        glyph.width = tgt_w;
        glyph.height = tgt_h;
        glyph.rendermode = m_rendermode;
        glyph.outline_thickness = m_outline_thickness;
        glyph.offset_x = ft_glyph_left;
        glyph.offset_y = ft_glyph_top;
        glyph.s0 = x / (float)m_atlas->m_width;
        glyph.t0 = y / (float)m_atlas->m_height;
        glyph.s1 = (x + glyph.width) / (float)m_atlas->m_width;
        glyph.t1 = (y + glyph.height) / (float)m_atlas->m_height;

        // Discard hinting to get advance
        FT_Load_Glyph(face, glyph_index, FT_LOAD_RENDER | FT_LOAD_NO_HINTING);
        slot = face->glyph;
        glyph.advance_x = convert_F26Dot6_to_float(slot->advance.x);
        glyph.advance_y = convert_F26Dot6_to_float(slot->advance.y);

        m_glyphs.push_back(glyph);

        if (m_rendermode != RENDER_NORMAL && m_rendermode != RENDER_SIGNED_DISTANCE_FIELD)
            FT_Done_Glyph(ft_glyph);

        GenerateKerning(this, &library, &face);

        FT_Done_Face(face);
        FT_Done_FreeType(library);

        return 0;
    }

    size_t Font::LoadGlyphs(const char *codepoints) {
        for (size_t i = 0; i < strlen(codepoints); i += utf8_surrogate_len(codepoints + i))
            if (!LoadGlyph(codepoints + i))
                return utf8_strlen(codepoints + 1);
    }

    void Font::EnlargeAtlas(size_t newWidth, size_t newHeight) {
        assert(m_atlas);
        assert(newWidth >= m_atlas->m_width);
        assert(newHeight >= m_atlas->m_height);
        assert(newWidth + newHeight > m_atlas->m_width + m_atlas->m_height);

        TextureAtlas *at = m_atlas;
        size_t width_old = m_atlas->m_width;
        size_t height_old = m_atlas->m_height;

        uint8_t *data_old = m_atlas->m_data;
        at->m_data = (uint8_t *)malloc(newWidth * newHeight * at->m_depth);
        at->m_width = newWidth;
        at->m_height = newHeight;

        if (newWidth > width_old) {
            ivec3 node;
            node.x = width_old - 1;
            node.y = 1;
            node.z = newWidth - width_old;
            at->m_nodes.push_back(node);
        }

        //copy over data from the old buffer, skipping first row and column because of the margin
        size_t pixel_size = at->m_depth;
        size_t old_row_size = width_old * pixel_size;
        at->SetRegion(1, 1, width_old - 2, height_old - 2, data_old + old_row_size + pixel_size, old_row_size);
        free(data_old);
        //change uv coordinates of existing glyphs to reflect size change
        float mulw = (float)width_old / newWidth;
        float mulh = (float)height_old / newHeight;

        for (size_t i = 0; i < m_glyphs.size(); i++) {
            Glyph *g = &m_glyphs[i];
            g->s0 *= mulw;
            g->s1 *= mulw;
            g->t0 *= mulh;
            g->t1 *= mulh;
        }
    }

} // namespace ftbgfx