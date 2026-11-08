#include "Texture.hpp"

#include "../../define.h"

#include <cstddef>

#include <SDL3/SDL_video.h>

#ifdef SDL_PLATFORM_VITA
#include <GLES2/gl2.h>
#include "../../GLES2_typedefs.h"
#else
#include <SDL3/SDL_opengl.h>
#include <SDL3/SDL_opengl_glext.h>
#endif

namespace Game::Graphics::OpenGLFunctions {
    PFNGLGENTEXTURESPROC glGenTextures = NULL;
    PFNGLDELETETEXTURESPROC glDeleteTextures = NULL;

    PFNGLACTIVETEXTUREPROC glActiveTexture = NULL;
    PFNGLBINDTEXTUREPROC glBindTexture = NULL;

    PFNGLTEXIMAGE2DPROC glTexImage2D = NULL;
    PFNGLTEXPARAMETERIPROC glTexParameteri = NULL;

    PFNGLFRAMEBUFFERTEXTURE2DPROC glFramebufferTexture2D = NULL;
}

namespace Game::Graphics::Texture {

    Game::Graphics::Classes::Texture* (*Create)() = NULL;
    void (*CreateBulk)(size_t CreateAmount,
                       Game::Graphics::Classes::Texture* Textures[]) = NULL;
    void (*Unbind)() = NULL;

    void (*UploadPixelData)(size_t Width,
                            size_t Height,
                            unsigned int Format,
                            unsigned int PixelFormat,
                            const void* PixelData) = NULL;
    void (*SetFilteringMode)(unsigned int FilteringMode) = NULL;

    void (*AttachToFramebuffer)(unsigned int AttachmentType,
                                Game::Graphics::Classes::Texture* Texture) =
        NULL;
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

Game::Graphics::Classes::Texture*
    Game::Graphics::Texture::etc::_Texture_Create_OpenGL() {
    Game::Graphics::Classes::Texture_OpenGL* NewTexture =
        new Game::Graphics::Classes::Texture_OpenGL;
    Game::Graphics::OpenGLFunctions::glGenTextures(1, &NewTexture->GLObject);

    return reinterpret_cast<Game::Graphics::Classes::Texture*>(NewTexture);
}

void Game::Graphics::Texture::etc::_Texture_CreateBulk_OpenGL(
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

void Game::Graphics::Texture::etc::_Texture_Unbind_OpenGL() {
    Game::Graphics::OpenGLFunctions::glBindTexture(GL_TEXTURE_2D, 0);
}

void Game::Graphics::Texture::etc::_Texture_UploadPixelData(
    size_t Width,
    size_t Height,
    unsigned int Format,
    unsigned int PixelFormat,
    const void* PixelData) {
    Game::Graphics::OpenGLFunctions::glTexImage2D(GL_TEXTURE_2D, 0, Format,
                                                  Width, Height, 0, Format,
                                                  PixelFormat, PixelData);
}

void Game::Graphics::Texture::etc::_Texture_SetFilteringMode(
    unsigned int FilteringMode) {
    Game::Graphics::OpenGLFunctions::glTexParameteri(
        GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, FilteringMode);
    Game::Graphics::OpenGLFunctions::glTexParameteri(
        GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, FilteringMode);
}

void Game::Graphics::Texture::etc::_Texture_AttachToFramebuffer(
    unsigned int AttachmentType,
    Game::Graphics::Classes::Texture* Texture) {
    Game::Graphics::OpenGLFunctions::glFramebufferTexture2D(
        GL_FRAMEBUFFER, AttachmentType, GL_TEXTURE_2D,
        reinterpret_cast<Game::Graphics::Classes::Texture_OpenGL*>(Texture)
            ->GLObject,
        0);
}