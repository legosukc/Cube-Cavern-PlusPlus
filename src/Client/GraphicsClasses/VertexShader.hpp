#pragma once

#include "../../define.h"

#include <cstddef>

#ifdef SDL_PLATFORM_VITA
#include <GLES2/gl2.h>
#include "../../GLES2_typedefs.h"
#else
#include <SDL3/SDL_opengl.h>
#include <SDL3/SDL_opengl_glext.h>
#endif

#include "ShaderBase.hpp"

namespace Game::Graphics::Classes {
    class VertexShader : public Game::Graphics::Classes::ShaderBase {};
    class VertexShader_OpenGL : public Game::Graphics::Classes::ShaderBase_OpenGL<GL_VERTEX_SHADER> {};
}

namespace Game::Graphics::VertexShader {

    inline Game::Graphics::Classes::VertexShader* (*Create)() = NULL;
    inline void (*CreateBulk)(size_t CreateAmount,
                       Game::Graphics::Classes::VertexShader* VertexShaders[]) = NULL;

    inline void Init_OpenGL();
    inline void Init_Vulkan();
    inline void Init_Metal();
    inline void Init_DirectX11();
    inline void Init_DirectX12();
}

void Game::Graphics::VertexShader::Init_OpenGL() {
    using namespace Game::Graphics::ShaderBase;
    using VertexShader = Game::Graphics::Classes::VertexShader;
    using VertexShader_OpenGL = Game::Graphics::Classes::VertexShader_OpenGL;

    Game::Graphics::VertexShader::Create =
        Game::Graphics::ShaderBase::WrapperTemplates::Create_OpenGL<
            VertexShader, VertexShader_OpenGL>;
    Game::Graphics::VertexShader::CreateBulk =
        Game::Graphics::ShaderBase::WrapperTemplates::CreateBulk_OpenGL<
            VertexShader, VertexShader_OpenGL>;
}

void Game::Graphics::VertexShader::Init_Vulkan() {}

void Game::Graphics::VertexShader::Init_Metal() {}

void Game::Graphics::VertexShader::Init_DirectX11() {}

void Game::Graphics::VertexShader::Init_DirectX12() {}