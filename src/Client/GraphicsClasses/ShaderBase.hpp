#pragma once

#include "../../define.h"

#include <SDL3/SDL_opengl.h>
#include <SDL3/SDL_opengl_glext.h>
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