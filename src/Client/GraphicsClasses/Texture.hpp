#pragma once

#include "../../define.h"

#include <cstddef>

#include <SDL3/SDL_opengl.h>
#include <SDL3/SDL_opengl_glext.h>

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

    extern void (*AttachToFramebuffer)(unsigned int AttachmentType,
                                Game::Graphics::Classes::Texture* Texture);

	inline void Init_OpenGL();
    inline void Init_Vulkan();
    inline void Init_Metal();
    inline void Init_DirectX11();
    inline void Init_DirectX12();
}