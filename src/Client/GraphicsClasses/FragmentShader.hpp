#pragma once

#include "../../define.h"

#include <SDL3/SDL_platform_defines.h>

#ifdef SDL_PLATFORM_VITA
#include <GLES2/gl2.h>
#else
#include <SDL3/SDL_opengl.h>
#endif

#include "ShaderBase.hpp"

namespace Game::Graphics::Classes {

    class FragmentShader : public Game::Graphics::Classes::ShaderBase {};

    class FragmentShader_OpenGL
        : public Game::Graphics::Classes::ShaderBase_OpenGL<
              GL_FRAGMENT_SHADER> {
       public:
        virtual bool SetShaderSource(const char* Source) override;
        virtual bool Compile() override;
    };
}

namespace Game::Graphics::FragmentShader {

    extern Game::Graphics::Classes::FragmentShader* (*Create)();
    extern void (*CreateBulk)(
        size_t CreateAmount,
        Game::Graphics::Classes::FragmentShader* FragmentShaders[]);

    inline void Init_OpenGL();
    inline void Init_Vulkan();
    inline void Init_Metal();
    inline void Init_DirectX11();
    inline void Init_DirectX12();
}

void Game::Graphics::FragmentShader::Init_OpenGL() {
    using namespace Game::Graphics::ShaderBase;
    using FragmentShader = Game::Graphics::Classes::FragmentShader;
    using FragmentShader_OpenGL =
        Game::Graphics::Classes::FragmentShader_OpenGL;

    Game::Graphics::FragmentShader::Create =
        WrapperTemplates::Create_OpenGL<FragmentShader, FragmentShader_OpenGL>;
    Game::Graphics::FragmentShader::CreateBulk =
        WrapperTemplates::CreateBulk_OpenGL<FragmentShader,
                                            FragmentShader_OpenGL>;
}

void Game::Graphics::FragmentShader::Init_Vulkan() {}

void Game::Graphics::FragmentShader::Init_Metal() {}

void Game::Graphics::FragmentShader::Init_DirectX11() {}

void Game::Graphics::FragmentShader::Init_DirectX12() {}