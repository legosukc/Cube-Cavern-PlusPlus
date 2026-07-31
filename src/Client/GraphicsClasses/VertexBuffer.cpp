#include "../../define.h"

#include <SDL3/SDL_opengl.h>
#include <SDL3/SDL_opengl_glext.h>

#include "VertexBuffer.hpp"
#include "BufferBase.hpp"

bool Game::Graphics::Classes::VertexBuffer_OpenGL::IsBound() const {
    // TODO: FIX THIS!!!!
    return true;
}

namespace Game::Graphics::VertexBuffer {

    Game::Graphics::Classes::VertexBuffer* (*Create)();
    void (*CreateBulk)(size_t CreateAmount,
                       Game::Graphics::Classes::VertexBuffer* VertexBuffers[]);
    void (*Unbind)();
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