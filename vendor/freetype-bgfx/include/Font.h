//
// Created on May 2nd 2021 by loanselot1.
//

#pragma once

#include "Atlas.h"

#include <vector>

namespace ftbgfx {
    enum RenderMode { RENDER_NORMAL, RENDER_OUTLINE_EDGE, RENDER_OUTLINE_POSITIVE, RENDER_OUTLINE_NEGATIVE, RENDER_SIGNED_DISTANCE_FIELD };

    struct Kerning {
        /**
         * Left Unicode codepoint in the kern pair in UTF-32 LE encoding.
         */
        uint32_t codepoint;

        /**
         * Kerning value (in fractional pixels).
         */
        float kerning;
    };

    struct Glyph {
        /**
         * Unicode codepoint this glyph represents in UTF-32 LE encoding.
         */
        uint32_t codepoint;

        /**
         * Glyph's width in pixels.
         */
        size_t width;

        /**
         * Glyph's height in pixels.
         */
        size_t height;

        /**
         * Glyph's left bearing expressed in integer pixels.
         */
        int offset_x;

        /**
         * Glyphs's top bearing expressed in integer pixels.
         *
         * Remember that this is the distance from the baseline to the top-most
         * glyph scanline, upwards y coordinates being positive.
         */
        int offset_y;

        /**
         * For horizontal text layouts, this is the horizontal distance (in
         * fractional pixels) used to increment the pen position when the glyph is
         * drawn as part of a string of text.
         */
        float advance_x;

        /**
         * For vertical text layouts, this is the vertical distance (in fractional
         * pixels) used to increment the pen position when the glyph is drawn as
         * part of a string of text.
         */
        float advance_y;

        /**
         * First normalized texture coordinate (x) of top-left corner
         */
        float s0;

        /**
         * Second normalized texture coordinate (y) of top-left corner
         */
        float t0;

        /**
         * First normalized texture coordinate (x) of bottom-right corner
         */
        float s1;

        /**
         * Second normalized texture coordinate (y) of bottom-right corner
         */
        float t1;

        /**
         * A vector of kerning pairs relative to this glyph.
         */
        std::vector<Kerning> kernings;

        /**
         * Mode this glyph was rendered
         */
        RenderMode rendermode;

        /**
         * Glyph outline thickness
         */
        float outline_thickness;
    };

    class Font {
    public:
        Font(TextureAtlas *atlas, float sizePT, const char *path);
        Font(TextureAtlas *atlas, float sizePT, uint8_t *mem, size_t memSize);
        ~Font();

        Glyph *GetGlyph(const char *codepoint);
        Glyph *FindGlyph(const char *codepoint);
        int LoadGlyph(const char *codepoint);
        size_t LoadGlyphs(const char *codepoints);

        void EnlargeAtlas(size_t newWidth, size_t newHeight);

        float GetKerning(const char *codepoint);

    public:
        std::vector<Glyph> m_glyphs;
        TextureAtlas *m_atlas;

        /**
         * font location
         */
        enum {
            TEXTURE_FONT_FILE = 0,
            TEXTURE_FONT_MEMORY,
        } m_location;

        union {
            /**
             * Font filename, for when location == TEXTURE_FONT_FILE
             */
            char *m_filename;

            /**
             * Font memory address, for when location == TEXTURE_FONT_MEMORY
             */
            struct {
                const uint8_t *base;
                size_t size;
            } m_memory;
        };

        /**
         * Font size
         */
        float m_size;

        /**
         * Whether to use autohint when rendering font
         */
        int m_hinting;

        /**
         * Mode the font is rendering its next glyph
         */
        RenderMode m_rendermode;

        /**
         * Outline thickness
         */
        float m_outline_thickness;

        /**
         * Whether to use our own lcd filter.
         */
        int m_filtering;

        /**
         * LCD filter weights
         */
        unsigned char m_lcd_weights[5];

        /**
         * Whether to use kerning if available
         */
        int m_kerning;

        /**
         * This field is simply used to compute a default line spacing (i.e., the
         * baseline-to-baseline distance) when writing text with this font. Note
         * that it usually is larger than the sum of the ascender and descender
         * taken as absolute values. There is also no guarantee that no glyphs
         * extend above or below subsequent baselines when using this distance.
         */
        float m_height;

        /**
         * This field is the distance that must be placed between two lines of
         * text. The baseline-to-baseline distance should be computed as:
         * ascender - descender + linegap
         */
        float m_linegap;

        /**
         * The ascender is the vertical distance from the horizontal baseline to
         * the highest 'character' coordinate in a font face. Unfortunately, font
         * formats define the ascender differently. For some, it represents the
         * ascent of all capital latin characters (without accents), for others it
         * is the ascent of the highest accented character, and finally, other
         * formats define it as being equal to bbox.yMax.
         */
        float m_ascender;

        /**
         * The descender is the vertical distance from the horizontal baseline to
         * the lowest 'character' coordinate in a font face. Unfortunately, font
         * formats define the descender differently. For some, it represents the
         * descent of all capital latin characters (without accents), for others it
         * is the ascent of the lowest accented character, and finally, other
         * formats define it as being equal to bbox.yMin. This field is negative
         * for values below the baseline.
         */
        float m_descender;

        /**
         * The position of the underline line for this face. It is the center of
         * the underlining stem. Only relevant for scalable formats.
         */
        float m_underline_position;

        /**
         * The thickness of the underline for this face. Only relevant for scalable
         * formats.
         */
        float m_underline_thickness;

        /**
        * The padding to be add to the glyph's texture that are loaded by this font.
        * Usefull when adding effects with shaders.
        */
        int m_padding;
    };
} // namespace ftbgfx
