#include "Font.h"

#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_STROKER_H
// #include FT_ADVANCES_H
#include FT_LCD_FILTER_H
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
            case 0: error = FT_New_Face(*library, self->m_filename, 0, face); break;
            case 1: error = FT_New_Memory_Face(*library, self->m_memory.base, self->m_memory.size, 0, face); break;
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

} // namespace ftbgfx