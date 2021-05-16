#include "pch.hh"

#include "ShaderManager.hh"

#include <bgfx/bgfx.h>

void ShaderManager::AppendShader( Identifier const &rIdent,
                                  const bgfx::RendererType::Enum eRenderer,
                                  const ShaderType eType, uint8_t *pSrc,
                                  uint32_t uCodeLen )
{
    ZoneScoped;
    m_vShaders.push_back( Shader { rIdent, eRenderer, eType, pSrc, uCodeLen } );
}

bgfx::ShaderHandle ShaderManager::LoadShader( Identifier const &rIdent,
                                              const ShaderType pShaderType )
{
    ZoneScoped;
    Shader *p_shader = nullptr;

    for ( auto &shader : m_vShaders )
    {
        if ( shader.m_sIdentifier == rIdent && shader.m_eType == pShaderType
             && shader.m_eRenderer == bgfx::getRendererType( ) )
        {
            p_shader = &shader;
            break;
        }
    }

    if ( p_shader == nullptr ) return BGFX_INVALID_HANDLE;

    const auto *buffer = bgfx::copy( p_shader->m_pCode, p_shader->m_uCodeLen );

    const auto handle = bgfx::createShader( buffer );
    bgfx::setName( handle, rIdent );

    return handle;
}

bgfx::ProgramHandle ShaderManager::LoadProgram( Identifier const &rIdent )
{
    ZoneScoped;
    if ( m_umPrograms.find( rIdent ) != m_umPrograms.end( ) )
        return m_umPrograms [ rIdent ];

    const auto vsh = LoadShader( rIdent, ShaderType::Vertex );
    const auto fsh = LoadShader( rIdent, ShaderType::Fragment );

    const auto handle = bgfx::createProgram( vsh, fsh, true );

    m_umPrograms [ rIdent ] = handle;

    return handle;
}

#include "Core/Graphics/tempsh/fs_default.d3d11.h"
#include "Core/Graphics/tempsh/fs_default.d3d12.h"
#include "Core/Graphics/tempsh/fs_default.d3d9.h"
#include "Core/Graphics/tempsh/fs_default.glsl.h"
#include "Core/Graphics/tempsh/fs_default.metal.h"
#include "Core/Graphics/tempsh/fs_default.vulkan.h"
#include "Core/Graphics/tempsh/vs_default.d3d11.h"
#include "Core/Graphics/tempsh/vs_default.d3d12.h"
#include "Core/Graphics/tempsh/vs_default.d3d9.h"
#include "Core/Graphics/tempsh/vs_default.glsl.h"
#include "Core/Graphics/tempsh/vs_default.metal.h"
#include "Core/Graphics/tempsh/vs_default.vulkan.h"

void ShaderManager::LoadDefaultShaders( )
{
    ZoneScoped;
    AppendShader( "engine://default", bgfx::RendererType::Direct3D9,
                  ShaderType::Fragment, (uint8_t *) fs_default_d3d9,
                  sizeof( fs_default_d3d9 ) );
    AppendShader( "engine://default", bgfx::RendererType::Direct3D11,
                  ShaderType::Fragment, (uint8_t *) fs_default_d3d11,
                  sizeof( fs_default_d3d11 ) );
    AppendShader( "engine://default", bgfx::RendererType::Direct3D12,
                  ShaderType::Fragment, (uint8_t *) fs_default_d3d12,
                  sizeof( fs_default_d3d12 ) );
    AppendShader( "engine://default", bgfx::RendererType::OpenGL,
                  ShaderType::Fragment, (uint8_t *) fs_default_glsl,
                  sizeof( fs_default_glsl ) );
    AppendShader( "engine://default", bgfx::RendererType::Metal,
                  ShaderType::Fragment, (uint8_t *) fs_default_metal,
                  sizeof( fs_default_metal ) );
    AppendShader( "engine://default", bgfx::RendererType::Vulkan,
                  ShaderType::Fragment, (uint8_t *) fs_default_vulkan,
                  sizeof( fs_default_vulkan ) );
    AppendShader( "engine://default", bgfx::RendererType::Direct3D9,
                  ShaderType::Vertex, (uint8_t *) vs_default_d3d9,
                  sizeof( vs_default_d3d9 ) );
    AppendShader( "engine://default", bgfx::RendererType::Direct3D11,
                  ShaderType::Vertex, (uint8_t *) vs_default_d3d11,
                  sizeof( vs_default_d3d11 ) );
    AppendShader( "engine://default", bgfx::RendererType::Direct3D12,
                  ShaderType::Vertex, (uint8_t *) vs_default_d3d12,
                  sizeof( vs_default_d3d12 ) );
    AppendShader( "engine://default", bgfx::RendererType::OpenGL,
                  ShaderType::Vertex, (uint8_t *) vs_default_glsl,
                  sizeof( vs_default_glsl ) );
    AppendShader( "engine://default", bgfx::RendererType::Metal,
                  ShaderType::Vertex, (uint8_t *) vs_default_metal,
                  sizeof( vs_default_metal ) );
    AppendShader( "engine://default", bgfx::RendererType::Vulkan,
                  ShaderType::Vertex, (uint8_t *) vs_default_vulkan,
                  sizeof( vs_default_vulkan ) );
}
