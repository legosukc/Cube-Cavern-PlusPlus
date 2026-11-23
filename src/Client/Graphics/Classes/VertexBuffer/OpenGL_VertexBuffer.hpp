#pragma once

#include "../BufferBase/OpenGL_BufferBase.hpp"

#include "../VertexBuffer.hpp"

namespace Game::Graphics::Classes {
    class VertexBuffer_OpenGL : public BufferBase_OpenGL<GL_ARRAY_BUFFER> {
       public:
        virtual bool IsBound() const override;
    };
}

namespace Game::Graphics::VertexBuffer::OpenGL {

    inline void Init() {
        using namespace Game::Graphics::BufferBase;
        using VertexBuffer = Game::Graphics::Classes::VertexBuffer;
        using VertexBuffer_OpenGL =
            Game::Graphics::Classes::VertexBuffer_OpenGL;

        Game::Graphics::VertexBuffer::Create =
            BufferBase::OpenGL::Wrappers::Create<VertexBuffer,
                                                 VertexBuffer_OpenGL>;
        Game::Graphics::VertexBuffer::CreateBulk =
            BufferBase::OpenGL::Wrappers::CreateBulk<VertexBuffer,
                                                     VertexBuffer_OpenGL>;

        Game::Graphics::VertexBuffer::Unbind =
            BufferBase::OpenGL::Wrappers::Unbind<VertexBuffer_OpenGL>;
    }
}