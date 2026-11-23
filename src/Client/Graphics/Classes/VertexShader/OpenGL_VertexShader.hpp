#pragma once

#include "../../../../define.h"

#ifdef SDL_PLATFORM_VITA
#include <GLES2/gl2.h>
#else
#include <SDL3/SDL_opengl.h>
#endif

#include "../ShaderBase/OpenGL_ShaderBase.hpp"
#include "Base_VertexShader.hpp"

#include "../VertexShader.hpp"

namespace Game::Graphics::Classes {
    class VertexShader_OpenGL
        : public Game::Graphics::Classes::ShaderBase_OpenGL<GL_VERTEX_SHADER> {
    };
}

namespace Game::Graphics::VertexShader::OpenGL {
    inline void Init() {
        using namespace Game::Graphics::ShaderBase;
        using VertexShader = Game::Graphics::Classes::VertexShader;
        using VertexShader_OpenGL =
            Game::Graphics::Classes::VertexShader_OpenGL;

        Game::Graphics::VertexShader::Create =
            Game::Graphics::ShaderBase::WrapperTemplates::Create_OpenGL<
                VertexShader, VertexShader_OpenGL>;
        Game::Graphics::VertexShader::CreateBulk =
            Game::Graphics::ShaderBase::WrapperTemplates::CreateBulk_OpenGL<
                VertexShader, VertexShader_OpenGL>;
    }
}