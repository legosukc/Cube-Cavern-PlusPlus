#pragma once

#include "../../define.h"

#include <SDL3/SDL_video.h>

#include "BufferBase/OpenGL_BufferBase.hpp"

namespace Game::Graphics::BufferBase {
    inline void Init_OpenGL();
    inline void Init_Vulkan();
    inline void Init_Metal();
    inline void Init_DirectX11();
    inline void Init_DirectX12();
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