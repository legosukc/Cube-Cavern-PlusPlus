#pragma once

#include "../../../define.h"

#ifdef SDL_PLATFORM_VITA
#include <GLES2/gl2.h>
#include "../../../GLES2_typedefs.h"
#else
#include <SDL3/SDL_opengl.h>
#include <SDL3/SDL_opengl_glext.h>
#include <SDL3/SDL_opengles2_gl2ext.h>
#endif

#include "Base_Texture.hpp"

#include "../BaseClassDefinitions.hpp"

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

namespace Game::Graphics::Texture::Wrappers {
    Game::Graphics::Classes::Texture* Create_OpenGL();

    void CreateBulk_OpenGL(size_t CreateAmount,
                           Game::Graphics::Classes::Texture* Textures[]);

    void Unbind_OpenGL();

    void UploadPixelData_OpenGL(size_t Width,
                                size_t Height,
                                unsigned int Format,
                                unsigned int PixelFormat,
                                const void* PixelData);

    void SetFilteringMode_OpenGL(unsigned int FilteringMode);

    void AttachToFramebuffer_OpenGL(unsigned int AttachmentType,
                                    Game::Graphics::Classes::Texture* Texture);
}