#include "../../define.h"

#include "UniformBuffer.hpp"

#include <SDL3/SDL_video.h>
#include <SDL3/SDL_opengl.h>
#include <SDL3/SDL_opengl_glext.h>

namespace Game::Graphics::OpenGLFunctions {
    PFNGLBINDBUFFERBASEPROC glBindBufferBase;
    PFNGLBINDBUFFERRANGEPROC glBindBufferRange;
}

Game::Graphics::Classes::UniformBuffer::UniformBuffer() = default;
void Game::Graphics::Classes::UniformBuffer::BindBase(GLuint Index) {}

void Game::Graphics::Classes::UniformBuffer_OpenGL::BindBase(GLuint Index) {
    Game::Graphics::OpenGLFunctions::glBindBufferBase(GL_UNIFORM_BUFFER, Index,
                                                      this->GLObject);
}

bool Game::Graphics::Classes::UniformBuffer_OpenGL::IsBound() const {
    // TODO: FIX THIS!!!!
    return true;
}

namespace Game::Graphics::UniformBuffer {

    Game::Graphics::Classes::UniformBuffer* (*Create)();
    void (*CreateBulk)(
        size_t CreateAmount,
        Game::Graphics::Classes::UniformBuffer* UniformBuffers[]);
    void (*Unbind)();
}

void Game::Graphics::UniformBuffer::Init_OpenGL() {
    LOAD_OPENGL_FUNCTION(glBindBufferBase);
    LOAD_OPENGL_FUNCTION(glBindBufferRange);

    using namespace Game::Graphics::BufferBase;
    using UniformBuffer = Game::Graphics::Classes::UniformBuffer;
    using UniformBuffer_OpenGL = Game::Graphics::Classes::UniformBuffer_OpenGL;

    Game::Graphics::UniformBuffer::Create =
        WrapperTemplates::Create_OpenGL<UniformBuffer, UniformBuffer_OpenGL>;
    Game::Graphics::UniformBuffer::CreateBulk =
        WrapperTemplates::CreateBulk_OpenGL<UniformBuffer,
                                            UniformBuffer_OpenGL>;

    Game::Graphics::UniformBuffer::Unbind =
        WrapperTemplates::Unbind_OpenGL<UniformBuffer_OpenGL>;
}

void Game::Graphics::UniformBuffer::Init_Vulkan() {}

void Game::Graphics::UniformBuffer::Init_Metal() {}

void Game::Graphics::UniformBuffer::Init_DirectX11() {}

void Game::Graphics::UniformBuffer::Init_DirectX12() {}