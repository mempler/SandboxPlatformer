#pragma once

#include <string_view>
#include <unordered_map>

#include "Core/Graphics/Texture2D.hh"
#include "Core/Utils/Identifier.hh"

enum class ShaderType : uint8_t { Fragment = 0, Vertex = 1 };

class ShaderManager {
public:
    // Internal function, can be used but shouldn't! use at your own risk!
    void AppendShader(Identifier const &rIdent, const bgfx::RendererType::Enum eRenderer, const ShaderType eType, const std::vector<uint8_t> &vSrc);

    // Internal function, can be used but shouldn't! use at your own risk!
    bgfx::ShaderHandle LoadShader(Identifier const &rIdent, const ShaderType eShaderType);

    bgfx::ProgramHandle LoadProgram(Identifier const &rIdent);

private:
    struct Shader {
        Identifier m_sIdentifier;
        bgfx::RendererType::Enum m_eRenderer;
        ShaderType m_eType;

        std::vector<uint8_t> m_vCode;
    };

    std::unordered_map<Identifier, bgfx::ProgramHandle> m_umPrograms;

    std::vector<Shader> m_vShaders;
};