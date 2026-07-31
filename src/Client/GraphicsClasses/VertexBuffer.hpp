#ifndef GRAPHICS_VERTEXBUFFER_H
#define GRAPHICS_VERTEXBUFFER_H

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

    Game::Graphics::Classes::VertexBuffer* (*Create)();
    void (*CreateBulk)(size_t CreateAmount,
                       Game::Graphics::Classes::VertexBuffer* VertexBuffers[]);
    void (*Unbind)();

    inline void Init_OpenGL();
    inline void Init_Vulkan();
    inline void Init_Metal();
    inline void Init_DirectX11();
    inline void Init_DirectX12();
}

bool Game::Graphics::Classes::VertexBuffer_OpenGL::IsBound() const {
    // TODO: FIX THIS!!!!
    return true;
}

void Game::Graphics::VertexBuffer::Init_OpenGL() {
    using namespace Game::Graphics::BufferBase;
    using VertexBuffer = Game::Graphics::Classes::VertexBuffer;
    using VertexBuffer_OpenGL = Game::Graphics::Classes::VertexBuffer_OpenGL;

    Game::Graphics::VertexBuffer::Create =
        WrapperTemplates::Create_OpenGL<VertexBuffer, VertexBuffer_OpenGL>;
    Game::Graphics::VertexBuffer::CreateBulk =
        WrapperTemplates::CreateBulk_OpenGL<VertexBuffer, VertexBuffer_OpenGL>;

    Game::Graphics::VertexBuffer::Unbind =
        WrapperTemplates::Unbind_OpenGL<VertexBuffer_OpenGL>;
}

void Game::Graphics::VertexBuffer::Init_Vulkan() {}

void Game::Graphics::VertexBuffer::Init_Metal() {}

void Game::Graphics::VertexBuffer::Init_DirectX11() {}

void Game::Graphics::VertexBuffer::Init_DirectX12() {}

#endif