#include "ShaderBase.hpp"

#include "../../define.h"

#ifdef SDL_PLATFORM_VITA
#include <GLES2/gl2.h>
#include "../../GLES2_typedefs.h"
#else
#include <SDL3/SDL_opengl.h>
#include <SDL3/SDL_opengl_glext.h>
#endif

#include <SDL3/SDL_video.h>

namespace Game::Graphics::OpenGLFunctions {
    PFNGLCREATESHADERPROC glCreateShader = NULL;
    PFNGLDELETESHADERPROC glDeleteShader = NULL;

    PFNGLSHADERSOURCEPROC glShaderSource = NULL;
    PFNGLGETSHADERSOURCEPROC glGetShaderSource = NULL;

    PFNGLCOMPILESHADERPROC glCompileShader = NULL;

    PFNGLGETSHADERIVPROC glGetShaderiv = NULL;
    PFNGLGETSHADERINFOLOGPROC glGetShaderInfoLog = NULL;
}

Game::Graphics::Classes::ShaderBase::~ShaderBase() = default;

bool Game::Graphics::Classes::ShaderBase::SetShaderSource(const char* Source) {
    return false;
}

bool Game::Graphics::Classes::ShaderBase::Compile() {
    return false;
}

template <GLenum ShaderType>
Game::Graphics::Classes::ShaderBase_OpenGL<ShaderType>::~ShaderBase_OpenGL() {
    if (this->GLObject == 0) {
#ifdef DEBUG_BUILD
        std::cerr << "Attempted to free a unallocated OpenGL Shader."
                  << std::endl;
        __debugbreak();
#endif
        return;
    }
    Game::Graphics::OpenGLFunctions::glDeleteShader(this->GLObject);
}

template <GLenum ShaderType>
bool Game::Graphics::Classes::ShaderBase_OpenGL<ShaderType>::SetShaderSource(
    const char* Source) {
    Game::Graphics::OpenGLFunctions::glShaderSource(this->GLObject, 1, &Source,
                                                    NULL);

    return true;
}

template <GLenum ShaderType>
bool Game::Graphics::Classes::ShaderBase_OpenGL<ShaderType>::Compile() {
    Game::Graphics::OpenGLFunctions::glCompileShader(this->GLObject);
    return true;
}

template Game::Graphics::Classes::ShaderBase_OpenGL<
    GL_VERTEX_SHADER>::~ShaderBase_OpenGL();

template bool Game::Graphics::Classes::ShaderBase_OpenGL<
    GL_VERTEX_SHADER>::SetShaderSource(const char* Source);

template bool Game::Graphics::Classes::ShaderBase_OpenGL<
    GL_VERTEX_SHADER>::Compile();

template Game::Graphics::Classes::ShaderBase_OpenGL<
    GL_FRAGMENT_SHADER>::~ShaderBase_OpenGL();

template bool Game::Graphics::Classes::ShaderBase_OpenGL<
    GL_FRAGMENT_SHADER>::SetShaderSource(const char* Source);

template bool
    Game::Graphics::Classes::ShaderBase_OpenGL<GL_FRAGMENT_SHADER>::Compile();