#pragma once

#include "../../define.h"

#include <SDL3/SDL_opengl.h>
#include <SDL3/SDL_opengl_glext.h>

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