#pragma once

#include <SDL3/SDL_platform_defines.h>

#include "Base_Program.hpp"

#include "../Program.hpp"

namespace Game::Graphics::OpenGLFunctions {

    extern PFNGLCREATEPROGRAMPROC glCreateProgram;
    extern PFNGLDELETEPROGRAMPROC glDeleteProgram;

    extern PFNGLGETUNIFORMLOCATIONPROC glGetUniformLocation;

#ifndef SDL_PLATFORM_VITA
    extern PFNGLGETUNIFORMBLOCKINDEXPROC glGetUniformBlockIndex;
    extern PFNGLUNIFORMBLOCKBINDINGPROC glUniformBlockBinding;
#endif

    extern PFNGLATTACHSHADERPROC glAttachShader;
    extern PFNGLGETATTACHEDSHADERSPROC glGetAttachedShaders;
    extern PFNGLDETACHSHADERPROC glDetachShader;

    extern PFNGLLINKPROGRAMPROC glLinkProgram;
    extern PFNGLUSEPROGRAMPROC glUseProgram;

    extern PFNGLGETPROGRAMIVPROC glGetProgramiv;

    extern PFNGLUNIFORM1IPROC glUniform1i;
    extern PFNGLUNIFORM2IVPROC glUniform2iv;
    extern PFNGLUNIFORM2IVPROC glUniform3iv;
    extern PFNGLUNIFORM2IVPROC glUniform4iv;

#ifndef SDL_PLATFORM_VITA
    extern PFNGLUNIFORM1UIPROC glUniform1ui;
    extern PFNGLUNIFORM2UIVPROC glUniform2uiv;
    extern PFNGLUNIFORM2UIVPROC glUniform3uiv;
    extern PFNGLUNIFORM2UIVPROC glUniform4uiv;
#endif

    extern PFNGLUNIFORM1FPROC glUniform1f;
    extern PFNGLUNIFORM2FVPROC glUniform2fv;
    extern PFNGLUNIFORM2FVPROC glUniform3fv;
    extern PFNGLUNIFORM2FVPROC glUniform4fv;

    extern PFNGLUNIFORMMATRIX2FVPROC glUniformMatrix2fv;
    extern PFNGLUNIFORMMATRIX3FVPROC glUniformMatrix3fv;
    extern PFNGLUNIFORMMATRIX4FVPROC glUniformMatrix4fv;

    extern void (*glGetProgramBinary)(GLuint program,
                                      GLsizei bufSize,
                                      GLsizei* length,
                                      GLenum* binaryFormat,
                                      void* binary);
}

// TODO: make the SetUniformBvec_ functions use a Bvec_ struct.

namespace Game::Graphics::Program::OpenGL::Wrappers {

    Game::Graphics::Classes::Program* Create();

    void SetUniformSint32(Sint32 UniformIndex, Sint32 Value);
    void SetUniformUint32(Sint32 UniformIndex, Uint32 Value);
    void SetUniformBool(Sint32 UniformIndex, bool Value);
    void SetUniformFloat(Sint32 UniformIndex, float Value);

    template <class BoolVectorType>
    void SetUniformBvec_(Sint32 UniformIndex, const BoolVectorType& BVector);

    template <class VectorType>
    void SetUniform_vec2(Sint32 UniformIndex, const VectorType& Vector);

    template <class VectorType>
    void SetUniform_vec3(Sint32 UniformIndex, const VectorType& Vector);

    template <class VectorType>
    void SetUniform_vec4(Sint32 UniformIndex, const VectorType& Vector);

    void SetUniformMat2(Sint32 UniformIndex, const Math::Mat2& Mat2);

    void SetUniformMat3(Sint32 UniformIndex, const Math::Mat3& Mat3);

    void SetUniformMat4(Sint32 UniformIndex, const Math::Mat4& Mat4);
}

namespace Game::Graphics::Classes {

    class Program_OpenGL : public Program {
       public:
        virtual ~Program_OpenGL() override;

        virtual int GetUniformLocation(const char* UniformName) const override;

        virtual bool Link(bool Cache = true) override;
        virtual void Use() override;
    };
}

namespace Game::Graphics::Program::OpenGL {
    inline void Init() {
        LOAD_OPENGL_FUNCTION(glCreateProgram);
        LOAD_OPENGL_FUNCTION(glDeleteProgram);

        LOAD_OPENGL_FUNCTION(glGetUniformLocation);

#ifndef SDL_PLATFORM_VITA
        LOAD_OPENGL_FUNCTION(glGetUniformBlockIndex);
        LOAD_OPENGL_FUNCTION(glUniformBlockBinding);
#endif

        LOAD_OPENGL_FUNCTION(glAttachShader);
        LOAD_OPENGL_FUNCTION(glGetAttachedShaders);
        LOAD_OPENGL_FUNCTION(glDetachShader);

        LOAD_OPENGL_FUNCTION(glLinkProgram);
        LOAD_OPENGL_FUNCTION(glUseProgram);

        LOAD_OPENGL_FUNCTION(glGetProgramiv);

        LOAD_OPENGL_FUNCTION(glUniform1i);
        LOAD_OPENGL_FUNCTION(glUniform2iv);
        LOAD_OPENGL_FUNCTION(glUniform3iv);
        LOAD_OPENGL_FUNCTION(glUniform4iv);

#ifndef SDL_PLATFORM_VITA
        LOAD_OPENGL_FUNCTION(glUniform1ui);
        LOAD_OPENGL_FUNCTION(glUniform2uiv);
        LOAD_OPENGL_FUNCTION(glUniform3uiv);
        LOAD_OPENGL_FUNCTION(glUniform4uiv);
#endif

        LOAD_OPENGL_FUNCTION(glUniform1f);
        LOAD_OPENGL_FUNCTION(glUniform2fv);
        LOAD_OPENGL_FUNCTION(glUniform3fv);
        LOAD_OPENGL_FUNCTION(glUniform4fv);

        LOAD_OPENGL_FUNCTION(glUniformMatrix2fv);
        LOAD_OPENGL_FUNCTION(glUniformMatrix3fv);
        LOAD_OPENGL_FUNCTION(glUniformMatrix4fv);

#ifdef SDL_PLATFORM_VITA
        OpenGLFunctions::glGetProgramBinary =
            (decltype(OpenGLFunctions::glGetProgramBinary))
                Game::Graphics::OpenGL::GetProcAddress("glGetProgramBinaryOES");
#else
        LOAD_OPENGL_FUNCTION(glGetProgramBinary);
#endif

        Graphics::Program::FuncPtrs::Create =
            Game::Graphics::Program::OpenGL::Wrappers::Create;

        Graphics::Program::FuncPtrs::SetUniformSint32 =
            Game::Graphics::Program::OpenGL::Wrappers::SetUniformSint32;
        Graphics::Program::FuncPtrs::SetUniformUint32 =
            Game::Graphics::Program::OpenGL::Wrappers::SetUniformUint32;
        Graphics::Program::FuncPtrs::SetUniformBool =
            Game::Graphics::Program::OpenGL::Wrappers::SetUniformBool;
        Graphics::Program::FuncPtrs::SetUniformFloat =
            Game::Graphics::Program::OpenGL::Wrappers::SetUniformFloat;

        Graphics::Program::FuncPtrs::SetUniformIvec2 =
            Game::Graphics::Program::OpenGL::Wrappers::SetUniform_vec2<
                Math::IVector2>;
        Graphics::Program::FuncPtrs::SetUniformUvec2 =
            Game::Graphics::Program::OpenGL::Wrappers::SetUniform_vec2<
                Math::UVector2>;
        Graphics::Program::FuncPtrs::SetUniformBvec2 =
            Game::Graphics::Program::OpenGL::Wrappers::SetUniform_vec2<
                Math::IVector2>;
        Graphics::Program::FuncPtrs::SetUniformFvec2 =
            Game::Graphics::Program::OpenGL::Wrappers::SetUniform_vec2<
                Math::Vector2>;

        Graphics::Program::FuncPtrs::SetUniformIvec3 =
            Game::Graphics::Program::OpenGL::Wrappers::SetUniform_vec3<
                Math::IVector3>;
        Graphics::Program::FuncPtrs::SetUniformUvec3 =
            Game::Graphics::Program::OpenGL::Wrappers::SetUniform_vec3<
                Math::UVector3>;
        Graphics::Program::FuncPtrs::SetUniformBvec3 =
            Game::Graphics::Program::OpenGL::Wrappers::SetUniform_vec3<
                Math::IVector3>;
        Graphics::Program::FuncPtrs::SetUniformFvec3 =
            Game::Graphics::Program::OpenGL::Wrappers::SetUniform_vec3<
                Math::Vector3>;

        Graphics::Program::FuncPtrs::SetUniformIvec4 =
            Game::Graphics::Program::OpenGL::Wrappers::SetUniform_vec4<
                Math::IVector4>;
        Graphics::Program::FuncPtrs::SetUniformUvec4 =
            Game::Graphics::Program::OpenGL::Wrappers::SetUniform_vec4<
                Math::UVector4>;
        Graphics::Program::FuncPtrs::SetUniformBvec4 =
            Game::Graphics::Program::OpenGL::Wrappers::SetUniform_vec4<
                Math::IVector4>;
        Graphics::Program::FuncPtrs::SetUniformFvec4 =
            Game::Graphics::Program::OpenGL::Wrappers::SetUniform_vec4<
                Math::Vector4>;

        Graphics::Program::FuncPtrs::SetUniformMat2 =
            Game::Graphics::Program::OpenGL::Wrappers::SetUniformMat2;
        Graphics::Program::FuncPtrs::SetUniformMat3 =
            Game::Graphics::Program::OpenGL::Wrappers::SetUniformMat3;
        Graphics::Program::FuncPtrs::SetUniformMat4 =
            Game::Graphics::Program::OpenGL::Wrappers::SetUniformMat4;
    }
}