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
}

VertexBatcher::~VertexBatcher() {
}

void VertexBatcher::BeginFrame() {
}

void VertexBatcher::EndFrame() {
    Reset();
}

void VertexBatcher::Flush() {
    for (auto &&event : m_vBatchEvents) {
        Texture2D *texture = event.first;
        auto &vertexes = event.second.vertices;
        auto &indexes = event.second.indexes;

        if (vertexes.size() > 0) {
            bgfx::TransientVertexBuffer tvb;
            bgfx::allocTransientVertexBuffer(&tvb, vertexes.size(), m_vlDefaultLayout);
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

void VertexBatcher::Reset() {
    Flush();

    // TODO: Add FreeSTL function
    // clear the map
    // for (auto &&event : m_vBatchEvents) FreeSTL(event.second.verts);

    // m_vBatchEvents.clear();
    // FreeSTL(m_vBatchEvents);
}