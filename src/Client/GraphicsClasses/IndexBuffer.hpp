#pragma once

#include <SDL3/SDL_platform_defines.h>

#ifdef SDL_PLATFORM_VITA
#include <GLES2/gl2.h>
#include "../../GLES2_typedefs.h"
#else
#include <SDL3/SDL_opengl.h>
#endif

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

void Game::Graphics::IndexBuffer::Init_OpenGL() {
    using namespace Game::Graphics::BufferBase;
    using IndexBuffer = Game::Graphics::Classes::IndexBuffer;
    using IndexBuffer_OpenGL = Game::Graphics::Classes::IndexBuffer_OpenGL;

    Game::Graphics::IndexBuffer::Create =
        Game::Graphics::BufferBase::WrapperTemplates::Create_OpenGL<
            IndexBuffer, IndexBuffer_OpenGL>;
    Game::Graphics::IndexBuffer::CreateBulk =
        Game::Graphics::BufferBase::WrapperTemplates::CreateBulk_OpenGL<
            IndexBuffer, IndexBuffer_OpenGL>;

    Game::Graphics::IndexBuffer::Unbind =
        Game::Graphics::BufferBase::WrapperTemplates::Unbind_OpenGL<
            IndexBuffer_OpenGL>;
}

void Game::Graphics::IndexBuffer::Init_Vulkan() {}

void Game::Graphics::IndexBuffer::Init_Metal() {}

void Game::Graphics::IndexBuffer::Init_DirectX11() {}

void Game::Graphics::IndexBuffer::Init_DirectX12() {}