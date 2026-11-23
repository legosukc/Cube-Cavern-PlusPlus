#pragma once

#include "Base_ShaderBase.hpp"

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

    template <GLenum ShaderType>
    class ShaderBase_OpenGL : virtual public ShaderBase {
       public:
        virtual ~ShaderBase_OpenGL() override;

        constexpr static inline GLenum OpenGLShaderType = ShaderType;

        virtual bool SetShaderSource(const char* Source) override;
        virtual bool Compile() override;
    };
}

namespace Game::Graphics::ShaderBase::WrapperTemplates {

    template <class ShaderClass, class OpenGLShaderClass>
    ShaderClass* Create_OpenGL();

    template <class ShaderClass, class OpenGLShaderClass>
    void CreateBulk_OpenGL(size_t CreateAmount, ShaderClass* Shaders[]);
}

namespace Game::Graphics::ShaderBase::OpenGL {
    inline void Init() {
        LOAD_OPENGL_FUNCTION(glCreateShader);
        LOAD_OPENGL_FUNCTION(glDeleteShader);

        LOAD_OPENGL_FUNCTION(glShaderSource);
        LOAD_OPENGL_FUNCTION(glGetShaderSource);

        LOAD_OPENGL_FUNCTION(glCompileShader);

        LOAD_OPENGL_FUNCTION(glGetShaderiv);
        LOAD_OPENGL_FUNCTION(glGetShaderInfoLog);
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