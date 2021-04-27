#include "pch.hh"

#include "ShaderManager.hh"

#include "bgfx/bgfx.h"

void ShaderManager::AppendShader(Identifier const &rIdent, const bgfx::RendererType::Enum eRenderer, const ShaderType eType, const std::vector<uint8_t> &vSrc) {
    m_vShaders.push_back(Shader{ rIdent, eRenderer, eType, vSrc });
}

bgfx::ShaderHandle ShaderManager::LoadShader(Identifier const &rIdent, const ShaderType pShaderType) {
    Shader *p_shader = nullptr;

    for (auto &shader : m_vShaders) {
        if (shader.m_sIdentifier == rIdent && shader.m_eType == pShaderType && shader.m_eRenderer == bgfx::getRendererType()) {
            p_shader = &shader;
            break;
        }
    }

    if (p_shader == nullptr)
        return BGFX_INVALID_HANDLE;

    const auto *buffer = bgfx::copy(p_shader->m_vCode.data(), static_cast<uint32_t>(p_shader->m_vCode.size()));

    const auto handle = bgfx::createShader(buffer);
    bgfx::setName(handle, rIdent.Raw().data());

    return handle;
}

bgfx::ProgramHandle ShaderManager::LoadProgram(Identifier const &rIdent) {
    if (m_umPrograms.find(rIdent) != m_umPrograms.end())
        return m_umPrograms[rIdent];

    const auto vsh = LoadShader(rIdent, ShaderType::Vertex);
    const auto fsh = LoadShader(rIdent, ShaderType::Fragment);

    const auto handle = bgfx::createProgram(vsh, fsh, true);

    m_umPrograms[rIdent] = handle;

    return handle;
}