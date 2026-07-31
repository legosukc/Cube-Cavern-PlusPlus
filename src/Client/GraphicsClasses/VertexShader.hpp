#pragma once

#include "../../define.h"

#include <SDL3/SDL_opengl.h>
#include <SDL3/SDL_opengl_glext.h>

#include "ShaderBase.hpp"

namespace Game::Graphics::Classes {
    class VertexShader : public Game::Graphics::Classes::ShaderBase {};
    class VertexShader_OpenGL : public Game::Graphics::Classes::ShaderBase_OpenGL<GL_VERTEX_SHADER> {};
}

namespace Game::Graphics::VertexShader {

    extern Game::Graphics::Classes::VertexShader* (*Create)();
    extern void (*CreateBulk)(size_t CreateAmount,
                       Game::Graphics::Classes::VertexShader* VertexShaders[]);

    inline void Init_OpenGL();
    inline void Init_Vulkan();
    inline void Init_Metal();
    inline void Init_DirectX11();
    inline void Init_DirectX12();
}