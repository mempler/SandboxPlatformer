//
// Created on May 2nd 2021 by loanselot1.
//

#pragma once

#include "vec234.h"

#include <vector>

namespace ftbgfx {
    class TextureAtlas {
    public:
        /**
         * Creates a new empty texture atlas.
         *
         * @param   width   width of the atlas
         * @param   height  height of the atlas
         * @param   depth   bit depth of the atlas
         * @return          a new empty texture atlas.
         *
         */
        TextureAtlas(size_t width, size_t height, size_t depth);
        ~TextureAtlas();

        /**
         *  Allocate a new region in the atlas.
         *
         *  @param width  width of the region to allocate
         *  @param height height of the region to allocate
         *  @return       Coordinates of the allocated region
         *
         */
        ivec4 GetRegion(size_t width, size_t height);

        /**
         *  Upload data to the specified atlas region.
         *
         *  @param self   a texture atlas structure
         *  @param x      x coordinate the region
         *  @param y      y coordinate the region
         *  @param width  width of the region
         *  @param height height of the region
         *  @param data   data to be uploaded into the specified region
         *  @param stride stride of the data
         *
         */
        void SetRegion(size_t x, size_t y, size_t width, size_t height, uint8_t *data, size_t stride);

        /**
         *  Remove all allocated regions from the atlas.
         *
         *  @param self   a texture atlas structure
         */
        void Clear();

        int Fits(size_t idx, size_t width, size_t height);
        void Merge();
        
        /**
        * Allocated nodes
        */
        std::vector<ivec3> m_nodes;

        /**
         *  Width (in pixels) of the underlying texture
         */
        size_t m_width;

        /**
         * Height (in pixels) of the underlying texture
         */
        size_t m_height;

        /**
         * Depth (in bytes) of the underlying texture
         */
        size_t m_depth;

        /**
         * Allocated surface size
         */
        size_t m_used;

        /**
         * Texture identity (OpenGL)
         */
        unsigned int m_id;

        /**
         * Atlas data
         */
        unsigned char *m_data;
    };
} // namespace ftbgfx
