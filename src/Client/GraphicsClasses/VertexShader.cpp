#include "../../define.h"

#include "VertexShader.hpp"

#include <SDL3/SDL_opengl.h>
#include <SDL3/SDL_opengl_glext.h>

namespace Game::Graphics::VertexShader {

    Game::Graphics::Classes::VertexShader* (*Create)();
    void (*CreateBulk)(size_t CreateAmount,
                       Game::Graphics::Classes::VertexShader* VertexShaders[]);
}

void Game::Graphics::VertexShader::Init_OpenGL() {
    using namespace Game::Graphics::ShaderBase;
    using VertexShader = Game::Graphics::Classes::VertexShader;
    using VertexShader_OpenGL = Game::Graphics::Classes::VertexShader_OpenGL;

    Game::Graphics::VertexShader::Create =
        Game::Graphics::ShaderBase::WrapperTemplates::Create_OpenGL<VertexShader,
                                                        VertexShader_OpenGL>;
    Game::Graphics::VertexShader::CreateBulk =
        Game::Graphics::ShaderBase::WrapperTemplates::CreateBulk_OpenGL<
            VertexShader, VertexShader_OpenGL>;
}

void Game::Graphics::VertexShader::Init_Vulkan() {}

void Game::Graphics::VertexShader::Init_Metal() {}

void Game::Graphics::VertexShader::Init_DirectX11() {}

void Game::Graphics::VertexShader::Init_DirectX12() {}