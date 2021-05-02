#include "Atlas.h"

#include "Utils.h"

#include <assert.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

#include <string>

namespace ftbgfx {
    TextureAtlas::TextureAtlas(size_t width, size_t height, size_t depth) : m_used(0), m_width(width), m_height(height), m_depth(depth), m_id(0) {
        // We want a one pixel border around the whole atlas to avoid any artefact when
        // sampling texture
        ivec3 node = { { 1, 1, width - 2 } };
        assert((depth == 1) || (depth == 3) || (depth == 4));

        m_nodes.push_back(node);
        m_data = (uint8_t *)malloc(width * height * depth);
        if (!m_data) {
            log_error("line %d: No more memory for allocating data\n", __LINE__);
        }
    }

    TextureAtlas::~TextureAtlas() {
        m_nodes.clear();

        if (m_data)
            free(m_data);
    }

    ivec4 TextureAtlas::GetRegion(size_t width, size_t height) {
        int y, best_index;
        size_t best_height, best_width;
        ivec3 *node, *prev;
        ivec4 region = { { 0, 0, width, height } };

        best_height = UINT_MAX;
        best_index = -1;
        best_width = UINT_MAX;

        for (size_t i = 0; i < m_nodes.size(); ++i) {
            y = Fits(i, width, height);
            if (y >= 0) {
                node = &m_nodes[i];
                if (((y + height) < best_height) || (((y + height) == best_height) && (node->z > 0 && (size_t)node->z < best_width))) {
                    best_height = y + height;
                    best_index = i;
                    best_width = node->z;
                    region.x = node->x;
                    region.y = y;
                }
            }
        }

        if (best_index == -1) {
            region.x = -1;
            region.y = -1;
            region.width = 0;
            region.height = 0;
            return region;
        }

        ivec3 v;
        v.x = region.x;
        v.y = region.y + height;
        v.z = width,

        m_nodes.insert(m_nodes.begin() + best_index, v);

        for (size_t i = best_index + 1; i < m_nodes.size(); ++i) {
            node = &m_nodes[i];
            prev = &m_nodes[i - 1];

            if (node->x < (prev->x + prev->z)) {
                int shrink = prev->x + prev->z - node->x;
                node->x += shrink;
                node->z -= shrink;
                if (node->z <= 0) {
                    m_nodes.erase(m_nodes.begin() + i);
                    --i;
                } else {
                    break;
                }
            } else {
                break;
            }
        }

        Merge();
        m_used += width * height;
        return region;
    }

    void TextureAtlas::SetRegion(size_t x, size_t y, size_t width, size_t height, uint8_t *data, size_t stride) {
        size_t depth;

        assert(x > 0);
        assert(y > 0);
        assert(x < (m_width - 1));
        assert((x + width) <= (m_width - 1));
        assert(y < (m_height - 1));
        assert((y + height) <= (m_height - 1));

        // prevent copying data from undefined position
        // and prevent memcpy's undefined behavior when count is zero
        assert(height == 0 || (data != NULL && width > 0));

        for (size_t i = 0; i < height; ++i) memcpy(m_data + ((y + i) * m_width + x) * depth, data + (i * stride), width * depth);
    }

    void TextureAtlas::Clear() {
        ivec3 node = { { 1, 1, 1 } };

        assert(m_data);

        m_nodes.clear();
        m_used = 0;
        // We want a one pixel border around the whole atlas to avoid any artefact when
        // sampling texture
        node.z = m_width - 2;

        m_nodes.push_back(node);

        memset(m_data, 0, m_width * m_height * m_depth);
    }

    int TextureAtlas::Fits(size_t idx, size_t width, size_t height) {
        ivec3 *node = &m_nodes[idx];
        int x = node->x;
        int y = node->y;
        int width_left = width;

        if ((x + width) > (m_width - 1))
            return -1;

        y = node->y;

        size_t i = idx;
        while (width_left > 0) {
            node = &m_nodes[i];
            if (node->y > y)
                y = node->y;

            if ((y + height) > (m_height - 1))
                return -1;

            width_left -= node->z;
            i++;
        }

        return y;
    }

    void TextureAtlas::Merge() {
        ivec3 *node, *next;

        for (size_t i = 0; i < m_nodes.size() - 1; i++) {
            node = &m_nodes[i];
            next = &m_nodes[i + 1];
            if (node->y == next->y) {
                node->z += next->z;
                m_nodes.erase(m_nodes.begin() + i + 1);
                i--;
            }
        }
    }
} // namespace ftbgfx