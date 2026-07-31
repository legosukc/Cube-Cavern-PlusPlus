
#include "../../define.h"

#include "ShaderBase.hpp"

#include <type_traits>

#include <SDL3/SDL_opengl_glext.h>
#include <SDL3/SDL_video.h>

namespace Game::Graphics::OpenGLFunctions {

    PFNGLCREATESHADERPROC glCreateShader;
    PFNGLDELETESHADERPROC glDeleteShader;

    PFNGLSHADERSOURCEPROC glShaderSource;
    PFNGLGETSHADERSOURCEPROC glGetShaderSource;

    PFNGLCOMPILESHADERPROC glCompileShader;

    PFNGLGETSHADERIVPROC glGetShaderiv;
    PFNGLGETSHADERINFOLOGPROC glGetShaderInfoLog;
}

namespace Game::Graphics::Classes {

    Game::Graphics::Classes::ShaderBase::~ShaderBase() = default;

    bool Game::Graphics::Classes::ShaderBase::SetShaderSource(
        const char* Source) {
        return false;
    }

    bool Game::Graphics::Classes::ShaderBase::Compile() {
        return false;
    }

    template <GLenum ShaderType>
    Game::Graphics::Classes::ShaderBase_OpenGL<
        ShaderType>::~ShaderBase_OpenGL() {
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
    bool
        Game::Graphics::Classes::ShaderBase_OpenGL<ShaderType>::SetShaderSource(
            const char* Source) {
        Game::Graphics::OpenGLFunctions::glShaderSource(this->GLObject, 1,
                                                        &Source, NULL);

        return true;
    }

    template <GLenum ShaderType>
    bool Game::Graphics::Classes::ShaderBase_OpenGL<ShaderType>::Compile() {
        Game::Graphics::OpenGLFunctions::glCompileShader(this->GLObject);
        return true;
    }
}

template <class ShaderClass, class OpenGLShaderClass>
ShaderClass* Game::Graphics::ShaderBase::WrapperTemplates::Create_OpenGL() {
    static_assert(
        std::is_base_of_v<Game::Graphics::Classes::ShaderBase, ShaderClass>,
        "Template class argument 'ShaderClass' isn't derived "
        "from 'Game::Graphics::Classes::ShaderBase'.");

    OpenGLShaderClass* NewShader = new OpenGLShaderClass;
    NewShader->GLObject = Game::Graphics::OpenGLFunctions::glCreateShader(
        OpenGLShaderClass::OpenGLShaderType);

    return reinterpret_cast<ShaderClass*>(NewShader);
}

template <class ShaderClass, class OpenGLShaderClass>
void Game::Graphics::ShaderBase::WrapperTemplates::CreateBulk_OpenGL(
    std::size_t CreateAmount,
    ShaderClass* Shaders[]) {
    static_assert(
        std::is_base_of_v<Game::Graphics::Classes::ShaderBase, ShaderClass>,
        "Template class argument 'ShaderClass' isn't derived "
        "from 'Game::Graphics::Classes::ShaderBase'.");

    OpenGLShaderClass* TempShader;
    std::size_t i;

    for (i = 0; i < CreateAmount; ++i) {
        TempShader = new OpenGLShaderClass;
        TempShader->GLObject = Game::Graphics::OpenGLFunctions::glCreateShader(
            OpenGLShaderClass::OpenGLShaderType);
        Shaders[i] = reinterpret_cast<ShaderClass*>(TempShader);
    }
}

void Game::Graphics::ShaderBase::Init_OpenGL() {
    LOAD_OPENGL_FUNCTION(glCreateShader);
    LOAD_OPENGL_FUNCTION(glDeleteShader);

    LOAD_OPENGL_FUNCTION(glShaderSource);
    LOAD_OPENGL_FUNCTION(glGetShaderSource);

    LOAD_OPENGL_FUNCTION(glCompileShader);

    LOAD_OPENGL_FUNCTION(glGetShaderiv);
    LOAD_OPENGL_FUNCTION(glGetShaderInfoLog);
}

void Game::Graphics::ShaderBase::Init_Vulkan() {}

void Game::Graphics::ShaderBase::Init_Metal() {}

void Game::Graphics::ShaderBase::Init_DirectX11() {}

void Game::Graphics::ShaderBase::Init_DirectX12() {}