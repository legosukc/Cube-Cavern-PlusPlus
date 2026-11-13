#pragma once

#include <SDL3/SDL_platform_defines.h>

#include "Base_Program.hpp"

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

namespace Game::Graphics::Program::Wrappers::OpenGL {

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

        virtual int GetUniformLocation(
            const char* UniformName) const override;

        virtual bool Link(bool Cache = true) override;
        virtual void Use() override;
    };
}