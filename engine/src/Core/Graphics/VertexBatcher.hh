#pragma once

#include "pch.hh"

#include "Core/Graphics/Texture2D.hh"
#include "Core/Managers/TextureManager.hh"

#include "bgfx/bgfx.h"

#include <glm/glm.hpp>

#include <vector>

// The idea behind using glm functions:
//
// (vertex)
// vec4[4] =
//     X X X X (X, Y, Z, W)
//     X X X X
//     X X X X
//     X X X X = mat4x4
//
// (uvs)
// vec2[4] =
//     X X . . (U, V)
//     X X . .
//     X X . .
//     X X . . = mat4x2

static const glm::mat4 g_m4DefPos = {
    1, 1, 1, 1,  // V1
    1, 0, 1, 1,  // V2
    0, 0, 1, 1,  // V3
    0, 1, 1, 1   // V4
};

static const glm::mat4x2 g_m4DefCoords = {
    1, 1,  // V1
    1, 0,  // V2
    0, 0,  // V3
    0, 1,  // V4
};

class ENGINE_EXPORT VertexBatcher
{
  public:
    /*****************************************************
     * VertexBatcher
     *
     * Tell this superior man to domiante speed in rendering.
     * It's too easy, just call it directly, no params required.
     * Who has time to mess with params anyways?
     *****************************************************/
    explicit VertexBatcher();

    /*****************************************************
     * ~VertexBatcher
     *
     * Why are you destroying it? Well good luck finding
     * better one...
     *****************************************************/
    ~VertexBatcher();

    /*****************************************************
     * Initialize
     *
     * Initialize what we need after Engine initialization
     * for example shader manager.
     *****************************************************/
    void Init( TextureManager &textureManager );

    /*****************************************************
     * BeginFrame
     *
     * Use this to take pre-rendering actions.
     *****************************************************/
    void BeginFrame();

    /*****************************************************
     * EndFrame
     *
     * End of all drawing, everything will be submited and
     * will be drawn into screen.
     *****************************************************/
    void EndFrame();

    /*****************************************************
     * Flush
     *
     * Force finish all events in cache.
     *****************************************************/
    void Flush();

    /*****************************************************
     * Reset
     *
     * Force finish all events in cache and restart again.
     *****************************************************/
    void Reset();

    /*****************************************************
     * Submit
     *
     * Add new event into queue but set UVs manually.
     *
     * @param pTexture Texture to be processed, set it to NULL for empty
     *texture.
     * @param m4Transform Matrix transformation of rectangle to be drawn.
     * @param m4UV Texture coordinates(matrix 4x2).
     * @param v4Color Color, use [0, 1].
     *****************************************************/
    void Submit( Texture2D *pTexture, const glm::mat4 &m4Transform,
                 const glm::mat4x2 &m4UV, const glm::vec4 &v4Color = { 1, 1, 1, 1 } );

    /*****************************************************
     * SubmitWithUV
     *
     * Add new event into queue but set pixel space UVs manually.
     *
     * @param pTexture Texture to be processed, set it to NULL for empty
     *texture.
     * @param m4Transform Matrix transformation of rectangle to be drawn.
     * @param v4UV Texture coordinates(x, y, w, h).
     * @param v4Color Color, use [0, 1].
     *****************************************************/
    void SubmitWithUV( Texture2D *pTexture, const glm::mat4 &m4Transform,
                       const glm::vec4 &v4UV, const glm::vec4 &v4Color = { 1, 1, 1, 1 } );

    /*****************************************************
     * SubmitWithRawUV
     *
     * Add new event into queue but set pre-calculated UVs manually.
     *
     * @param pTexture Texture to be processed, set it to NULL for empty
     *texture.
     * @param m4Transform Matrix transformation of rectangle to be drawn.
     * @param v4UV Texture coordinates(x, y, w, h).
     * @param v4Color Color, use [0, 1].
     *****************************************************/
    void SubmitWithRawUV( Texture2D *pTexture, const glm::mat4 &m4Transform,
                          const glm::vec4 &v4UV,
                          const glm::vec4 &v4Color = { 1, 1, 1, 1 } );

    /*****************************************************
     * Submit
     *
     * Quick function to not deal with UVs
     *
     * @param pTexture Texture to be processed, set it to NULL for empty
     *texture.
     * @param m4Transform Matrix transformation of rectangle to be drawn.
     * @param v4Color Color, use [0, 1].
     *****************************************************/
    void SubmitRectangle( Texture2D *pTexture, const glm::mat4 &m4Transform,
                          const glm::vec4 &v4Color = { 1, 1, 1, 1 } );
    void SubmitRectangleRawHandle( bgfx::TextureHandle hTexture,
                                   const glm::mat4 &m4Transform,
                                   const glm::vec4 &v4Color = { 1, 1, 1, 1 } );

    void SetCurrentView( bgfx::ViewId viID )
    {
        m_viCurrentView = viID;
    }

  private:  // Interestingly, we cannot define this bellow the function bellow,
            // GCC/CLANG doesn't like that on linux.
    struct VertexInfo
    {
        glm::vec3 pos {};
        glm::vec2 uv {};
        glm::vec4 color {};
    };

    struct BatchEvent
    {
        std::vector<VertexInfo> vertices {};
        uint32_t indexes = 0;
    };

  private:  // same goes for variables, it'll simply not compile.
    std::vector<std::pair<bgfx::TextureHandle, BatchEvent>> m_vBatchEvents {};

    bgfx::VertexLayout m_vlDefaultLayout;
    bgfx::IndexBufferHandle m_hIndexBufferHandle;
    bgfx::ProgramHandle m_hDefaultProgramHandle;
    bgfx::UniformHandle m_hTextureUniform;

    // We will use this to render screen to texture, this
    // will solve screen flickering while taking screenshots
    // using ShareX, well never tried it but thats the idea -l
    bgfx::FrameBufferHandle m_hScreenFrameBuffer;

    // Empty 1x1 white texture, useful for many things
    Texture2D *m_pWhiteTexture;

    bgfx::ViewId m_viCurrentView = 0;

  private:
    BatchEvent &GetVertexData( Texture2D *pTexture )
    {
        ZoneScoped;

        for ( auto &&t : m_vBatchEvents )
            if ( t.first.idx == pTexture->GetHandle().idx ) return t.second;

        m_vBatchEvents.push_back(
            std::make_pair( pTexture->GetHandle(), BatchEvent {} ) );

        m_vBatchEvents.back().second.vertices.reserve( 256 );  // Preallocate 256 vertices

        return m_vBatchEvents.back().second;
    }

    BatchEvent &GetVertexData( bgfx::TextureHandle &hTexture )
    {
        ZoneScoped;

        for ( auto &&t : m_vBatchEvents )
            if ( t.first.idx == hTexture.idx ) return t.second;

        m_vBatchEvents.push_back( std::make_pair( hTexture, BatchEvent {} ) );

        m_vBatchEvents.back().second.vertices.reserve( 256 );  // Preallocate 256 vertices

        return m_vBatchEvents.back().second;
    }
};