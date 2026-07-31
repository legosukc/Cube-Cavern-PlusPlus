#include "../../define.h"

#include <SDL3/SDL_video.h>
#include <SDL3/SDL_opengl.h>
#include <SDL3/SDL_opengl_glext.h>

#include "BufferBase.hpp"

namespace Game::Graphics::OpenGLFunctions {
    PFNGLGENBUFFERSPROC glGenBuffers;
    PFNGLDELETEBUFFERSPROC glDeleteBuffers;

    PFNGLBUFFERDATAPROC glBufferData;
    PFNGLBUFFERSUBDATAPROC glBufferSubData;

    PFNGLBINDBUFFERPROC glBindBuffer;
}

Game::Graphics::Classes::BufferBase::~BufferBase() = default;

void Game::Graphics::Classes::BufferBase::Bind() {}
bool Game::Graphics::Classes::BufferBase::IsBound() const {
    return false;
}

template <GLenum BufferTarget>
Game::Graphics::Classes::BufferBase_OpenGL<BufferTarget>::~BufferBase_OpenGL() {
    unlikely_branch if (this->GLObject == 0) {
#ifdef DEBUG_BUILD
        std::cerr << "Attempted to free an unallocated OpenGL buffer."
                  << std::endl;
        __debugbreak();
#endif
        return;
    }
    Game::Graphics::OpenGLFunctions::glDeleteBuffers(1, &this->GLObject);
}

template <GLenum BufferTarget>
void Game::Graphics::Classes::BufferBase_OpenGL<BufferTarget>::Bind() {
    Game::Graphics::OpenGLFunctions::glBindBuffer(BufferTarget, this->GLObject);
}

template <class BufferClass, class BufferOpenGLClass>
BufferClass* Game::Graphics::BufferBase::WrapperTemplates::Create_OpenGL() {
    BufferOpenGLClass* NewBuffer = new BufferOpenGLClass;
    Game::Graphics::OpenGLFunctions::glGenBuffers(1, &NewBuffer->GLObject);

    return reinterpret_cast<BufferClass*>(NewBuffer);
}

template <class BufferClass, class BufferOpenGLClass>
void Game::Graphics::BufferBase::WrapperTemplates::CreateBulk_OpenGL(
    size_t CreateAmount,
    BufferClass* Buffers[]) {
    for (size_t i = 0; i < CreateAmount; ++i) {
        Buffers[i] = reinterpret_cast<BufferClass*>(new BufferOpenGLClass());
        Game::Graphics::OpenGLFunctions::glGenBuffers(1, &Buffers[i]->GLObject);
    }
}

template <class BufferOpenGLClass>
void Game::Graphics::BufferBase::WrapperTemplates::Unbind_OpenGL() {
    Game::Graphics::OpenGLFunctions::glBindBuffer(
        BufferOpenGLClass::OpenGLBufferEnum, 0);
}

void Game::Graphics::BufferBase::Init_OpenGL() {
    LOAD_OPENGL_FUNCTION(glGenBuffers);
    LOAD_OPENGL_FUNCTION(glDeleteBuffers);

    LOAD_OPENGL_FUNCTION(glBindBuffer);

    LOAD_OPENGL_FUNCTION(glBufferData);
    LOAD_OPENGL_FUNCTION(glBufferSubData);
}

void Game::Graphics::BufferBase::Init_Vulkan() {}

void Game::Graphics::BufferBase::Init_Metal() {}

void Game::Graphics::BufferBase::Init_DirectX11() {}

void Game::Graphics::BufferBase::Init_DirectX12() {}