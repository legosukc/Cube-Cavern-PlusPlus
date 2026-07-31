#pragma once

#include "../../define.h"

#include <SDL3/SDL_opengl.h>
#include <SDL3/SDL_opengl_glext.h>

#include "BufferBase.hpp"

namespace Game::Graphics::Classes {

    class VertexBuffer : public Game::Graphics::Classes::BufferBase {};

    class VertexBuffer_OpenGL : public BufferBase_OpenGL<GL_ARRAY_BUFFER> {
       public:
        virtual bool IsBound() const override;
    };
}

namespace Game::Graphics::VertexBuffer {

    extern Game::Graphics::Classes::VertexBuffer* (*Create)();
    extern void (*CreateBulk)(
        size_t CreateAmount,
        Game::Graphics::Classes::VertexBuffer* VertexBuffers[]);
    extern void (*Unbind)();

    inline void Init_OpenGL();
    inline void Init_Vulkan();
    inline void Init_Metal();
    inline void Init_DirectX11();
    inline void Init_DirectX12();
}