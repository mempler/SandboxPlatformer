//
// Created on May 2nd 2021 by loanselot1.
//

#pragma once

namespace ftbgfx {
    struct ivec4 {
        int data[4];
        union {
            int x; /**< Alias for first component  */
            int y; /**< Alias for second component */
            int z; /**< Alias for third component  */
            int w; /**< Alias for fourht component */
        };
        union {
            int x_;     /**< Alias for first component  */
            int y_;     /**< Alias for second component */
            int width;  /**< Alias for third component  */
            int height; /**< Alias for fourth component */
        };
        union {
            int r; /**< Alias for first component  */
            int g; /**< Alias for second component */
            int b; /**< Alias for third component  */
            int a; /**< Alias for fourth component */
        };
        union {
            int red;   /**< Alias for first component  */
            int green; /**< Alias for second component */
            int blue;  /**< Alias for third component  */
            int alpha; /**< Alias for fourth component */
        };
        union {
            int vstart; /**< Alias for first component  */
            int vcount; /**< Alias for second component */
            int istart; /**< Alias for third component  */
            int icount; /**< Alias for fourth component */
        };
    };

    struct ivec3 {
        int data[3];
        union {
            int x; /**< Alias for first component  */
            int y; /**< Alias for second component */
            int z; /**< Alias for third component  */
        };
        union {
            int r; /**< Alias for first component  */
            int g; /**< Alias for second component */
            int b; /**< Alias for third component  */
        };
        union {
            int red;   /**< Alias for first component  */
            int green; /**< Alias for second component */
            int blue;  /**< Alias for third component  */
        };
    };

    struct ivec2 {
        int data[2];
        union {
            int x; /**< Alias for first component  */
            int y; /**< Alias for second component */
        };
        union {
            int s; /**< Alias for first component  */
            int t; /**< Alias for second component */
        };
        union {
            int start; /**< Alias for first component  */
            int end;   /**< Alias for second component */
        };
    };

    struct vec4 {
        float data[4];
        union {
            float x; /**< Alias for first component  */
            float y; /**< Alias for second component */
            float z; /**< Alias for third component  */
            float w; /**< Alias for fourht component */
        };
        union {
            float x_;     /**< Alias for first component  */
            float y_;     /**< Alias for second component */
            float width;  /**< Alias for third component  */
            float height; /**< Alias for fourth component */
        };
        union {
            float r; /**< Alias for first component  */
            float g; /**< Alias for second component */
            float b; /**< Alias for third component  */
            float a; /**< Alias for fourth component */
        };
        union {
            float red;   /**< Alias for first component  */
            float green; /**< Alias for second component */
            float blue;  /**< Alias for third component  */
            float alpha; /**< Alias for fourth component */
        };
        union {
            float vstart; /**< Alias for first component  */
            float vcount; /**< Alias for second component */
            float istart; /**< Alias for third component  */
            float icount; /**< Alias for fourth component */
        };
    };

    struct vec3 {
        float data[3];
        union {
            float x; /**< Alias for first component  */
            float y; /**< Alias for second component */
            float z; /**< Alias for third component  */
        };
        union {
            float r; /**< Alias for first component  */
            float g; /**< Alias for second component */
            float b; /**< Alias for third component  */
        };
        union {
            float red;   /**< Alias for first component  */
            float green; /**< Alias for second component */
            float blue;  /**< Alias for third component  */
        };
    };

    struct vec2 {
        float data[2];
        union {
            float x; /**< Alias for first component  */
            float y; /**< Alias for second component */
        };
        union {
            float s; /**< Alias for first component  */
            float t; /**< Alias for second component */
        };
        union {
            float start; /**< Alias for first component  */
            float end;   /**< Alias for second component */
        };
    };

} // namespace ftbgfx
