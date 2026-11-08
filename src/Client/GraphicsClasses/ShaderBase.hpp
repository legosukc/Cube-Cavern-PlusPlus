#pragma once

#include "../../define.h"

#include <type_traits>

#ifdef SDL_PLATFORM_VITA
#include <GLES2/gl2.h>
#include "../../GLES2_typedefs.h"
#else
#include <SDL3/SDL_opengl.h>
#include <SDL3/SDL_opengl_glext.h>
#endif

#include <SDL3/SDL_video.h>

#include "BaseClassDefinitions.hpp"

namespace Game::Graphics::OpenGLFunctions {
    extern PFNGLCREATESHADERPROC glCreateShader;
    extern PFNGLDELETESHADERPROC glDeleteShader;

    extern PFNGLSHADERSOURCEPROC glShaderSource;
    extern PFNGLGETSHADERSOURCEPROC glGetShaderSource;

    extern PFNGLCOMPILESHADERPROC glCompileShader;

    extern PFNGLGETSHADERIVPROC glGetShaderiv;
    extern PFNGLGETSHADERINFOLOGPROC glGetShaderInfoLog;
}

namespace Game::Graphics::Classes {

    class ShaderBase : public Game::Graphics::Classes::OpenGLClassBase {
       public:
        virtual ~ShaderBase();

        virtual bool SetShaderSource(const char* Source);
        virtual bool Compile();
    };

    template <GLenum ShaderType>
    class ShaderBase_OpenGL : virtual public ShaderBase {
       public:
        virtual ~ShaderBase_OpenGL() override;

        constexpr static inline GLenum OpenGLShaderType = ShaderType;

        virtual bool SetShaderSource(const char* Source) override;
        virtual bool Compile() override;
    };
}

namespace Game::Graphics::ShaderBase {

    namespace WrapperTemplates {

        template <class ShaderClass, class OpenGLShaderClass>
        ShaderClass* Create_OpenGL();

        template <class ShaderClass, class OpenGLShaderClass>
        void CreateBulk_OpenGL(size_t CreateAmount, ShaderClass* Shaders[]);
    }

    inline void Init_OpenGL();
    inline void Init_Vulkan();
    inline void Init_Metal();
    inline void Init_DirectX11();
    inline void Init_DirectX12();
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