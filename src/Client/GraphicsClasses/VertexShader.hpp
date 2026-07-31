#ifndef GRAPHICS_VERTEXSHADER_H
#define GRAPHICS_VERTEXSHADER_H

#include "../../define.h"

#include <SDL3/SDL_opengl.h>
#include <SDL3/SDL_opengl_glext.h>

#include "ShaderBase.hpp"

namespace Game::Graphics::Classes {
    class VertexShader : public Game::Graphics::Classes::ShaderBase {};
    class VertexShader_OpenGL : public Game::Graphics::Classes::ShaderBase_OpenGL<GL_VERTEX_SHADER> {};
}

namespace Game::Graphics::VertexShader {

    Game::Graphics::Classes::VertexShader* (*Create)();
    void (*CreateBulk)(size_t CreateAmount,
                       Game::Graphics::Classes::VertexShader* VertexShaders[]);

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

#endif