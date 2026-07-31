#pragma once

#include "../../define.h"

#include <SDL3/SDL_opengl.h>
#include <SDL3/SDL_opengl_glext.h>

#include "BufferBase.hpp"

namespace Game::Graphics::Classes {

    class IndexBuffer : public Game::Graphics::Classes::BufferBase {};

    class IndexBuffer_OpenGL
        : public Game::Graphics::Classes::BufferBase_OpenGL<
              GL_ELEMENT_ARRAY_BUFFER> {
       public:
        virtual bool IsBound() const override;
    };
}

namespace Game::Graphics::IndexBuffer {

    extern Game::Graphics::Classes::IndexBuffer* (*Create)();
    extern void (*CreateBulk)(
        size_t CreateAmount,
        Game::Graphics::Classes::IndexBuffer* IndexBuffers[]);
    extern void (*Unbind)();

    inline void Init_OpenGL();
    inline void Init_Vulkan();
    inline void Init_Metal();
    inline void Init_DirectX11();
    inline void Init_DirectX12();
}