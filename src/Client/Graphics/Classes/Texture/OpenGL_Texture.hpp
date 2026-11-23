#pragma once

#include "../../../../define.h"

#ifdef SDL_PLATFORM_VITA
#include <GLES2/gl2.h>
#include "../../../../GLES2_typedefs.h"
#else
#include <SDL3/SDL_opengl.h>
#include <SDL3/SDL_opengl_glext.h>
#include <SDL3/SDL_opengles2_gl2ext.h>
#endif

#include "Base_Texture.hpp"

#include "../BaseClassDefinitions.hpp"

#include "../Texture.hpp"

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

    class Texture_OpenGL : public Texture,
                           public Game::Graphics::Classes::OpenGLClassBase {
       public:
        virtual ~Texture_OpenGL() override;

        virtual void Bind(int BindIndex) override;
        virtual bool IsBound() const override;
    };
}

namespace Game::Graphics::Texture::OpenGL::Wrappers {

    Graphics::Classes::Texture* Create();
    void CreateBulk(size_t CreateAmount,
                    Graphics::Classes::Texture* Textures[]);

    void Unbind();

    void UploadPixelData(size_t Width,
                         size_t Height,
                         unsigned int Format,
                         unsigned int PixelFormat,
                         const void* PixelData);

    void SetFilteringMode(unsigned int FilteringMode);
    void AttachToFramebuffer(unsigned int AttachmentType,
                                    Graphics::Classes::Texture* Texture);
}

namespace Game::Graphics::Texture::OpenGL {
    inline void Init() {
        LOAD_OPENGL_FUNCTION(glGenTextures);
        LOAD_OPENGL_FUNCTION(glDeleteTextures);

        LOAD_OPENGL_FUNCTION(glActiveTexture);
        LOAD_OPENGL_FUNCTION(glBindTexture);

        LOAD_OPENGL_FUNCTION(glTexImage2D);
        LOAD_OPENGL_FUNCTION(glTexParameteri);

        LOAD_OPENGL_FUNCTION(glFramebufferTexture2D);

        Texture::Create = Wrappers::Create;
        Texture::CreateBulk = Wrappers::CreateBulk;

        Texture::Unbind = Wrappers::Unbind;

        Texture::UploadPixelData = Wrappers::UploadPixelData;
        Texture::SetFilteringMode = Wrappers::SetFilteringMode;

        Texture::AttachToFramebuffer = Wrappers::AttachToFramebuffer;
    }
}