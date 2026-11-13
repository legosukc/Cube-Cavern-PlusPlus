#pragma once

#include "../../define.h"

#include "Texture/Base_Texture.hpp"
#include "Texture/OpenGL_Texture.hpp"

namespace Game::Graphics::Texture {

    extern Game::Graphics::Classes::Texture* (*Create)();
    extern void (*CreateBulk)(size_t CreateAmount,
                              Game::Graphics::Classes::Texture* Textures[]);
    extern void (*Unbind)();

    extern void (*UploadPixelData)(size_t Width,
                                   size_t Height,
                                   unsigned int Format,
                                   unsigned int PixelFormat,
                                   const void* PixelData);
    extern void (*SetFilteringMode)(unsigned int FilteringMode);

    extern void (*AttachToFramebuffer)(
        unsigned int AttachmentType,
        Game::Graphics::Classes::Texture* Texture);

    inline void Init_OpenGL();
    inline void Init_Vulkan();
    inline void Init_Metal();
    inline void Init_DirectX11();
    inline void Init_DirectX12();
}

void Game::Graphics::Texture::Init_OpenGL() {
    LOAD_OPENGL_FUNCTION(glGenTextures);
    LOAD_OPENGL_FUNCTION(glDeleteTextures);

    LOAD_OPENGL_FUNCTION(glActiveTexture);
    LOAD_OPENGL_FUNCTION(glBindTexture);

    LOAD_OPENGL_FUNCTION(glTexImage2D);
    LOAD_OPENGL_FUNCTION(glTexParameteri);

    LOAD_OPENGL_FUNCTION(glFramebufferTexture2D);

    Texture::Create = Wrappers::Create_OpenGL;
    Texture::CreateBulk = Wrappers::CreateBulk_OpenGL;

    Texture::Unbind = Wrappers::Unbind_OpenGL;

    Texture::UploadPixelData = Wrappers::UploadPixelData_OpenGL;
    Texture::SetFilteringMode = Wrappers::SetFilteringMode_OpenGL;

    Texture::AttachToFramebuffer = Wrappers::AttachToFramebuffer_OpenGL;
}

void Game::Graphics::Texture::Init_Vulkan() {}

void Game::Graphics::Texture::Init_Metal() {}

void Game::Graphics::Texture::Init_DirectX11() {}

void Game::Graphics::Texture::Init_DirectX12() {}