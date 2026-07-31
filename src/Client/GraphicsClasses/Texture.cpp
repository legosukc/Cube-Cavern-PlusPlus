#include "../../define.h"

#include "Texture.hpp"

#include <cstddef>

#include <SDL3/SDL_video.h>
#include <SDL3/SDL_opengl.h>
#include <SDL3/SDL_opengl_glext.h>

namespace Game::Graphics::OpenGLFunctions {

    PFNGLGENTEXTURESPROC glGenTextures;
    PFNGLDELETETEXTURESPROC glDeleteTextures;

    PFNGLACTIVETEXTUREPROC glActiveTexture;
    PFNGLBINDTEXTUREPROC glBindTexture;

    PFNGLTEXIMAGE2DPROC glTexImage2D;
    PFNGLTEXPARAMETERIPROC glTexParameteri;

    PFNGLFRAMEBUFFERTEXTURE2DPROC glFramebufferTexture2D;
}

Game::Graphics::Classes::Texture::~Texture() = default;

void Game::Graphics::Classes::Texture::Bind() {}

bool Game::Graphics::Classes::Texture::IsBound() const {
    return false;
}

Game::Graphics::Classes::Texture_OpenGL::~Texture_OpenGL() {
    if (this->GLObject == 0) {
#ifdef DEBUG_BUILD
        std::cerr << "Attempted to free a unallocated OpenGL Texture."
                  << std::endl;
        __debugbreak();
#endif
        return;
    }
    Game::Graphics::OpenGLFunctions::glDeleteTextures(1, &this->GLObject);
}

void Game::Graphics::Classes::Texture_OpenGL::Bind() {
    Game::Graphics::OpenGLFunctions::glBindTexture(GL_TEXTURE_2D,
                                                   this->GLObject);
}

bool Game::Graphics::Classes::Texture_OpenGL::IsBound() const {
    // TODO: FIX THIS!!!!
    return true;
}

static Game::Graphics::Classes::Texture* _Texture_Create_OpenGL() {
    Game::Graphics::Classes::Texture_OpenGL* NewTexture =
        new Game::Graphics::Classes::Texture_OpenGL;
    Game::Graphics::OpenGLFunctions::glGenTextures(1, &NewTexture->GLObject);

    return reinterpret_cast<Game::Graphics::Classes::Texture*>(NewTexture);
}

static void _Texture_CreateBulk_OpenGL(
    size_t CreateAmount,
    Game::Graphics::Classes::Texture* Textures[]) {
    GLuint* TextureObjects = new GLuint[CreateAmount];
    Game::Graphics::Classes::Texture_OpenGL* TempTexture;

    Game::Graphics::OpenGLFunctions::glGenTextures(CreateAmount,
                                                   TextureObjects);
    for (int i = 0; i < CreateAmount; ++i) {
        TempTexture = new Game::Graphics::Classes::Texture_OpenGL;
        TempTexture->GLObject = TextureObjects[i];
        Textures[i] = TempTexture;
    }
    delete[] TextureObjects;
}

static void _Texture_Unbind_OpenGL() {
    Game::Graphics::OpenGLFunctions::glBindTexture(GL_TEXTURE_2D, 0);
}

static void _Texture_UploadPixelData(size_t Width,
                                     size_t Height,
                                     unsigned int Format,
                                     unsigned int PixelFormat,
                                     const void* PixelData) {
    Game::Graphics::OpenGLFunctions::glTexImage2D(GL_TEXTURE_2D, 0, Format,
                                                  Width, Height, 0, Format,
                                                  PixelFormat, PixelData);
}

static void _Texture_SetFilteringMode(unsigned int FilteringMode) {
    Game::Graphics::OpenGLFunctions::glTexParameteri(
        GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, FilteringMode);
    Game::Graphics::OpenGLFunctions::glTexParameteri(
        GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, FilteringMode);
}

static void _Texture_AttachToFramebuffer(
    unsigned int AttachmentType,
    Game::Graphics::Classes::Texture* Texture) {
    Game::Graphics::OpenGLFunctions::glFramebufferTexture2D(
        GL_FRAMEBUFFER, AttachmentType, GL_TEXTURE_2D,
        reinterpret_cast<Game::Graphics::Classes::Texture_OpenGL*>(Texture)
            ->GLObject,
        0);
}

namespace Game::Graphics::Texture {

    Game::Graphics::Classes::Texture* (*Create)();
    void (*CreateBulk)(size_t CreateAmount,
                       Game::Graphics::Classes::Texture* Textures[]);
    void (*Unbind)();

    void (*UploadPixelData)(size_t Width,
                            size_t Height,
                            unsigned int Format,
                            unsigned int PixelFormat,
                            const void* PixelData);
    void (*SetFilteringMode)(unsigned int FilteringMode);

    void (*AttachToFramebuffer)(unsigned int AttachmentType,
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

    Texture::Create = ::_Texture_Create_OpenGL;
    Texture::CreateBulk = ::_Texture_CreateBulk_OpenGL;

    Texture::Unbind = ::_Texture_Unbind_OpenGL;

    Texture::UploadPixelData = ::_Texture_UploadPixelData;
    Texture::SetFilteringMode = ::_Texture_SetFilteringMode;

    Texture::AttachToFramebuffer = ::_Texture_AttachToFramebuffer;
}

void Game::Graphics::Texture::Init_Vulkan() {}

void Game::Graphics::Texture::Init_Metal() {}

void Game::Graphics::Texture::Init_DirectX11() {}

void Game::Graphics::Texture::Init_DirectX12() {}