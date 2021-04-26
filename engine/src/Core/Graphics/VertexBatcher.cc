#include "VertexBatcher.hh"

static uint32_t g_uMaxQuads = 80000;

VertexBatcher::VertexBatcher() {
    m_vlDefaultLayout.begin()
        .add(bgfx::Attrib::Position, 2, bgfx::AttribType::Float)
        .add(bgfx::Attrib::TexCoord0, 2, bgfx::AttribType::Float)
        .add(bgfx::Attrib::Color0, 4, bgfx::AttribType::Float, true)
        .end();

    // We pre-alloc our index buffer to save some time in rendering loop
    uint32_t *quads = new uint32_t[g_uMaxQuads * 6];
    uint32_t offset = 0;

    for (uint32_t i = 0; i < (g_uMaxQuads * 6); i += 6) {
        quads[i + 0] = offset + 0;
        quads[i + 1] = offset + 1;
        quads[i + 2] = offset + 2;

        quads[i + 3] = offset + 2;
        quads[i + 4] = offset + 3;
        quads[i + 5] = offset + 0;

        offset += 4;
    }

    m_hIndexBufferHandle = bgfx::createIndexBuffer(bgfx::copy(quads, g_uMaxQuads * 6 * sizeof(uint32_t)), BGFX_BUFFER_INDEX32);
    delete[] quads;

    // Initialize uniforms here
    m_hTextureUniform = bgfx::createUniform("u_texture", bgfx::UniformType::Sampler);

    // Initialize programs here
    // m_hDefaultProgramHandle = GetShaderManager()->Load(...)
    uint32_t whiteTextureData = 0xffffffff;
    m_pWhiteTexture = &Texture2D::LoadRaw("White Texture", 1, 1, bgfx::TextureFormat::RGB8, (uint8_t *)&whiteTextureData, 4);
}

VertexBatcher::~VertexBatcher() {
}

/*****************************************************
 * BeginFrame
 *
 * Use this to take pre-rendering actions.
 *****************************************************/
void VertexBatcher::BeginFrame() {
}

/*****************************************************
 * EndFrame
 *
 * End of all drawing, everything will be submited and
 * will be drawn into screen.
 *****************************************************/
void VertexBatcher::EndFrame() {
    Reset();
}

/*****************************************************
 * Flush
 *
 * Force finish all events in cache.
 *****************************************************/
void VertexBatcher::Flush() {
    for (auto &&event : m_vBatchEvents) {
        Texture2D *texture = event.first;
        auto &vertexes = event.second.vertices;
        auto &indexes = event.second.indexes;

        if (vertexes.size() > 0) {
            bgfx::TransientVertexBuffer tvb;
            bgfx::allocTransientVertexBuffer(&tvb, vertexes.size(), m_vlDefaultLayout);

            // suggestion: use std::copy, somehow faster than this
            memcpy(tvb.data, &vertexes[0], vertexes.size() * sizeof(VertexInfo));

            bgfx::setVertexBuffer(0, &tvb, 0, vertexes.size());

            // This is wrong, nvm, not for now
            uint32_t count = indexes ? indexes : g_uMaxQuads * 6;
            bgfx::setIndexBuffer(m_hIndexBufferHandle, 0, count);
            bgfx::setTexture(0, m_hTextureUniform, texture->GetHandle());
            bgfx::setState(BGFX_STATE_WRITE_RGB | BGFX_STATE_WRITE_A | BGFX_STATE_BLEND_ALPHA | BGFX_STATE_MSAA);
            bgfx::submit(0, m_hDefaultProgramHandle);
        }
    }
}

/*****************************************************
 * Reset
 *
 * Force finish all events in cache and restart again.
 *****************************************************/
void VertexBatcher::Reset() {
    Flush();

    // TODO: Add FreeSTL function
    // clear the map
    // for (auto &&event : m_vBatchEvents) FreeSTL(event.second.verts);

    m_vBatchEvents.clear();
    // FreeSTL(m_vBatchEvents);
}

/*****************************************************
 * Submit
 *
 * Add new event into queue but set UVs manually.
 *****************************************************/
void VertexBatcher::Submit(Texture2D *pTexture, const glm::mat4 &m4Transform, const glm::vec4 &v4UV, const glm::vec4 &v4Color) {
    if (!pTexture)
        pTexture = m_pWhiteTexture;

    BatchEvent &event = GetVertexData(pTexture);
    event.vertices.resize(event.vertices.size() + 4);

    VertexInfo *info = &event.vertices[event.vertices.size() - 4];

    for (size_t i = 0; i < 4; i++) {
        info->pos = m4Transform * g_m4DefPos[i];
        info->color = v4Color;
        info->uv = g_m4DefCoords[i];
        info++;
    }

    event.indexes += 6;
}

/*****************************************************
 * Submit
 *
 * Quick function to not deal with UVs
 *****************************************************/
void VertexBatcher::SubmitRectangle(Texture2D *pTexture, const glm::mat4 &m4Transform, const glm::vec4 &v4Color) {
    Submit(pTexture, m4Transform, v4Color);
}
