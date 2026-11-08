#pragma once

#include "../../define.h"

#include <cstddef>

#include <SDL3/SDL_video.h>

#ifdef SDL_PLATFORM_VITA
#include <GLES2/gl2.h>
#include "../../GLES2_typedefs.h"
#else
#include <SDL3/SDL_opengl.h>
#include <SDL3/SDL_opengl_glext.h>
#include <SDL3/SDL_opengles2_gl2ext.h>
#endif

#include "BaseClassDefinitions.hpp"

namespace Game::Graphics::OpenGLFunctions {
    extern PFNGLGENTEXTURESPROC glGenTextures;
    extern PFNGLDELETETEXTURESPROC glDeleteTextures;

    extern PFNGLACTIVETEXTUREPROC glActiveTexture;
    extern PFNGLBINDTEXTUREPROC glBindTexture;

    extern PFNGLTEXIMAGE2DPROC glTexImage2D;
    extern PFNGLTEXPARAMETERIPROC glTexParameteri;

    extern PFNGLFRAMEBUFFERTEXTURE2DPROC glFramebufferTexture2D;
}

namespace Game::Graphics::Classes {

    class Texture : public Game::Graphics::Classes::OpenGLClassBase {
       public:
        virtual ~Texture();

        virtual void Bind();
        virtual bool IsBound() const;
    };

    class Texture_OpenGL : public Texture {
       public:
        virtual ~Texture_OpenGL() override;

        virtual void Bind() override;
        virtual bool IsBound() const override;
    };
}

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

namespace Game::Graphics::Texture::etc {
    Game::Graphics::Classes::Texture* _Texture_Create_OpenGL();

    void _Texture_CreateBulk_OpenGL(
        size_t CreateAmount,
        Game::Graphics::Classes::Texture* Textures[]);

    void _Texture_Unbind_OpenGL();

    void _Texture_UploadPixelData(size_t Width,
                                  size_t Height,
                                  unsigned int Format,
                                  unsigned int PixelFormat,
                                  const void* PixelData);

    void _Texture_SetFilteringMode(unsigned int FilteringMode);

    void _Texture_AttachToFramebuffer(
        unsigned int AttachmentType,
        Game::Graphics::Classes::Texture* Texture);
}

void Game::Graphics::Texture::Init_OpenGL() {
    LOAD_OPENGL_FUNCTION(glGenTextures);
    LOAD_OPENGL_FUNCTION(glDeleteTextures);

    LOAD_OPENGL_FUNCTION(glActiveTexture);
    LOAD_OPENGL_FUNCTION(glBindTexture);

    LOAD_OPENGL_FUNCTION(glTexImage2D);
    LOAD_OPENGL_FUNCTION(glTexParameteri);

    LOAD_OPENGL_FUNCTION(glFramebufferTexture2D);

    Texture::Create = etc::_Texture_Create_OpenGL;
    Texture::CreateBulk = etc::_Texture_CreateBulk_OpenGL;

    Texture::Unbind = etc::_Texture_Unbind_OpenGL;

    Texture::UploadPixelData = etc::_Texture_UploadPixelData;
    Texture::SetFilteringMode = etc::_Texture_SetFilteringMode;

    Texture::AttachToFramebuffer = etc::_Texture_AttachToFramebuffer;
}

void Game::Graphics::Texture::Init_Vulkan() {}

void Game::Graphics::Texture::Init_Metal() {}

void Game::Graphics::Texture::Init_DirectX11() {}

void Game::Graphics::Texture::Init_DirectX12() {}