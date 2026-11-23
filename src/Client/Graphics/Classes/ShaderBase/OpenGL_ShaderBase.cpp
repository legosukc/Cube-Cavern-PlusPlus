#include "OpenGL_ShaderBase.hpp"

namespace Game::Graphics::OpenGLFunctions {
    PFNGLCREATESHADERPROC glCreateShader = NULL;
    PFNGLDELETESHADERPROC glDeleteShader = NULL;

    PFNGLSHADERSOURCEPROC glShaderSource = NULL;
    PFNGLGETSHADERSOURCEPROC glGetShaderSource = NULL;

    PFNGLCOMPILESHADERPROC glCompileShader = NULL;

    PFNGLGETSHADERIVPROC glGetShaderiv = NULL;
    PFNGLGETSHADERINFOLOGPROC glGetShaderInfoLog = NULL;
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

template bool
    Game::Graphics::Classes::ShaderBase_OpenGL<GL_VERTEX_SHADER>::Compile();

template Game::Graphics::Classes::ShaderBase_OpenGL<
    GL_FRAGMENT_SHADER>::~ShaderBase_OpenGL();

template bool Game::Graphics::Classes::ShaderBase_OpenGL<
    GL_FRAGMENT_SHADER>::SetShaderSource(const char* Source);

template bool
    Game::Graphics::Classes::ShaderBase_OpenGL<GL_FRAGMENT_SHADER>::Compile();