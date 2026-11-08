#pragma once

// TODO: make the SetUniformBvec_ functions use a Bvec_ struct.

#include "../../define.h"

#ifdef SDL_PLATFORM_VITA
#include <GLES2/gl2.h>
#include "../../GLES2_typedefs.h"
#else
#include <SDL3/SDL_opengl.h>
#include <SDL3/SDL_opengl_glext.h>
#endif

#include <SDL3/SDL_video.h>

#include "../../MathClasses/Matrix.hpp"
#include "../../MathClasses/Vector2.hpp"
#include "../../MathClasses/Vector3.hpp"
#include "../../MathClasses/Vector4.hpp"

#include "BaseClassDefinitions.hpp"

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
    // extern PFNGLUNIFORM2IVPROC glUniform1iv;
    extern PFNGLUNIFORM2IVPROC glUniform2iv;
    extern PFNGLUNIFORM2IVPROC glUniform3iv;
    extern PFNGLUNIFORM2IVPROC glUniform4iv;

#ifndef SDL_PLATFORM_VITA
    extern PFNGLUNIFORM1UIPROC glUniform1ui;
    // extern PFNGLUNIFORM2UIVPROC glUniform1uiv;
    extern PFNGLUNIFORM2UIVPROC glUniform2uiv;
    extern PFNGLUNIFORM2UIVPROC glUniform3uiv;
    extern PFNGLUNIFORM2UIVPROC glUniform4uiv;
#endif

    extern PFNGLUNIFORM1FPROC glUniform1f;
    // extern PFNGLUNIFORM2FVPROC glUniform1fv;
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

namespace Game::Graphics::Classes {

    class Program : public Game::Graphics::Classes::OpenGLClassBase {
       public:
        virtual ~Program();

        virtual GLint GetUniformLocation(const char* UniformName) const;

        virtual bool Link(bool Cache = true);
        virtual void Use();
    };

    class Program_OpenGL : public Program {
       public:
        virtual ~Program_OpenGL() override;

        virtual GLint GetUniformLocation(
            const char* UniformName) const override;

        virtual bool Link(bool Cache = true) override;
        virtual void Use() override;
    };
}

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

// TODO: make the SetUniformBvec_ functions use a Bvec_ struct.

namespace Game::Graphics::Program {

    namespace FuncPtrs {
        extern Game::Graphics::Classes::Program* (*Create)();

        extern void (*SetUniformSint32)(Sint32 UniformIndex, Sint32 Value);
        extern void (*SetUniformUint32)(Sint32 UniformIndex, Uint32 Value);
        extern void (*SetUniformBool)(Sint32 UniformIndex, bool Value);
        extern void (*SetUniformFloat)(Sint32 UniformIndex, float Value);

        extern void (*SetUniformIvec2)(Sint32 UniformIndex,
                                       const Math::IVector2& Ivec2);
        extern void (*SetUniformUvec2)(Sint32 UniformIndex,
                                       const Math::UVector2& Uvec2);
        extern void (*SetUniformBvec2)(Sint32 UniformIndex,
                                       const Math::IVector2& Bvec2);
        extern void (*SetUniformFvec2)(Sint32 UniformIndex,
                                       const Math::Vector2& Fvec2);

        extern void (*SetUniformIvec3)(Sint32 UniformIndex,
                                       const Math::IVector3& Ivec3);
        extern void (*SetUniformUvec3)(Sint32 UniformIndex,
                                       const Math::UVector3& Uvec3);
        extern void (*SetUniformBvec3)(Sint32 UniformIndex,
                                       const Math::IVector3& Bvec3);
        extern void (*SetUniformFvec3)(Sint32 UniformIndex,
                                       const Math::Vector3& Fvec3);

        extern void (*SetUniformIvec4)(Sint32 UniformIndex,
                                       const Math::IVector4& Ivec4);
        extern void (*SetUniformUvec4)(Sint32 UniformIndex,
                                       const Math::UVector4& Uvec4);
        extern void (*SetUniformBvec4)(Sint32 UniformIndex,
                                       const Math::IVector4& Bvec4);
        extern void (*SetUniformFvec4)(Sint32 UniformIndex,
                                       const Math::Vector4& Fvec4);

        // TODO: figure out how to fix the errors when implementing these
        // variables in the .cpp
        extern void (*SetUniformMat2)(Sint32 UniformIndex,
                                      const Math::Mat2& Mat2);
        extern void (*SetUniformMat3)(Sint32 UniformIndex,
                                      const Math::Mat3& Mat3);
        extern void (*SetUniformMat4)(Sint32 UniformIndex,
                                      const Math::Mat4& Mat4);
    }

    inline Game::Graphics::Classes::Program* Create();

    inline void SetUniformSint32(Sint32 UniformIndex, Sint32 Value);
    inline void SetUniformUint32(Sint32 UniformIndex, Uint32 Value);
    inline void SetUniformBool(Sint32 UniformIndex, bool Value);
    inline void SetUniformFloat(Sint32 UniformIndex, float Value);

    inline void SetUniformIvec2(Sint32 UniformIndex,
                                const Math::IVector2& Ivec2);
    inline void SetUniformUvec2(Sint32 UniformIndex,
                                const Math::UVector2& Uvec2);
    inline void SetUniformBvec2(Sint32 UniformIndex,
                                const Math::IVector2& Bvec2);
    inline void SetUniformFvec2(Sint32 UniformIndex,
                                const Math::Vector2& Fvec2);

    inline void SetUniformIvec3(Sint32 UniformIndex,
                                const Math::IVector3& Ivec3);
    inline void SetUniformUvec3(Sint32 UniformIndex,
                                const Math::UVector3& Uvec3);
    inline void SetUniformBvec3(Sint32 UniformIndex,
                                const Math::IVector3& Bvec3);
    inline void SetUniformFvec3(Sint32 UniformIndex,
                                const Math::Vector3& Fvec3);

    inline void SetUniformIvec4(Sint32 UniformIndex,
                                const Math::IVector4& Ivec4);
    inline void SetUniformUvec4(Sint32 UniformIndex,
                                const Math::UVector4& Uvec4);
    inline void SetUniformBvec4(Sint32 UniformIndex,
                                const Math::IVector4& Bvec4);
    inline void SetUniformFvec4(Sint32 UniformIndex,
                                const Math::Vector4& Fvec4);

    inline void SetUniformMat2(Sint32 UniformIndex, const Math::Mat2& Mat2);
    inline void SetUniformMat3(Sint32 UniformIndex, const Math::Mat3& Mat3);
    inline void SetUniformMat4(Sint32 UniformIndex, const Math::Mat4& Mat4);

    inline void Init_OpenGL();
    inline void Init_Vulkan();
    inline void Init_Metal();
    inline void Init_DirectX11();
    inline void Init_DirectX12();
}

void Game::Graphics::Program::Init_OpenGL() {
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
        Game::Graphics::Program::Wrappers::OpenGL::Create;

    Graphics::Program::FuncPtrs::SetUniformSint32 =
        Game::Graphics::Program::Wrappers::OpenGL::SetUniformSint32;
    Graphics::Program::FuncPtrs::SetUniformUint32 =
        Game::Graphics::Program::Wrappers::OpenGL::SetUniformUint32;
    Graphics::Program::FuncPtrs::SetUniformBool =
        Game::Graphics::Program::Wrappers::OpenGL::SetUniformBool;
    Graphics::Program::FuncPtrs::SetUniformFloat =
        Game::Graphics::Program::Wrappers::OpenGL::SetUniformFloat;

    Graphics::Program::FuncPtrs::SetUniformIvec2 =
        Game::Graphics::Program::Wrappers::OpenGL::SetUniform_vec2<
            Math::IVector2>;
    Graphics::Program::FuncPtrs::SetUniformUvec2 =
        Game::Graphics::Program::Wrappers::OpenGL::SetUniform_vec2<
            Math::UVector2>;
    Graphics::Program::FuncPtrs::SetUniformBvec2 =
        Game::Graphics::Program::Wrappers::OpenGL::SetUniform_vec2<
            Math::IVector2>;
    Graphics::Program::FuncPtrs::SetUniformFvec2 =
        Game::Graphics::Program::Wrappers::OpenGL::SetUniform_vec2<
            Math::Vector2>;

    Graphics::Program::FuncPtrs::SetUniformIvec3 =
        Game::Graphics::Program::Wrappers::OpenGL::SetUniform_vec3<
            Math::IVector3>;
    Graphics::Program::FuncPtrs::SetUniformUvec3 =
        Game::Graphics::Program::Wrappers::OpenGL::SetUniform_vec3<
            Math::UVector3>;
    Graphics::Program::FuncPtrs::SetUniformBvec3 =
        Game::Graphics::Program::Wrappers::OpenGL::SetUniform_vec3<
            Math::IVector3>;
    Graphics::Program::FuncPtrs::SetUniformFvec3 =
        Game::Graphics::Program::Wrappers::OpenGL::SetUniform_vec3<
            Math::Vector3>;

    Graphics::Program::FuncPtrs::SetUniformIvec4 =
        Game::Graphics::Program::Wrappers::OpenGL::SetUniform_vec4<
            Math::IVector4>;
    Graphics::Program::FuncPtrs::SetUniformUvec4 =
        Game::Graphics::Program::Wrappers::OpenGL::SetUniform_vec4<
            Math::UVector4>;
    Graphics::Program::FuncPtrs::SetUniformBvec4 =
        Game::Graphics::Program::Wrappers::OpenGL::SetUniform_vec4<
            Math::IVector4>;
    Graphics::Program::FuncPtrs::SetUniformFvec4 =
        Game::Graphics::Program::Wrappers::OpenGL::SetUniform_vec4<
            Math::Vector4>;

    Graphics::Program::FuncPtrs::SetUniformMat2 =
        Game::Graphics::Program::Wrappers::OpenGL::SetUniformMat2;
    Graphics::Program::FuncPtrs::SetUniformMat3 =
        Game::Graphics::Program::Wrappers::OpenGL::SetUniformMat3;
    Graphics::Program::FuncPtrs::SetUniformMat4 =
        Game::Graphics::Program::Wrappers::OpenGL::SetUniformMat4;
}

void Game::Graphics::Program::Init_Vulkan() {}

void Game::Graphics::Program::Init_Metal() {}

void Game::Graphics::Program::Init_DirectX11() {}

void Game::Graphics::Program::Init_DirectX12() {}

Game::Graphics::Classes::Program* Game::Graphics::Program::Create() {
    return Graphics::Program::FuncPtrs::Create();
}

void Game::Graphics::Program::SetUniformSint32(Sint32 UniformIndex,
                                               Sint32 Value) {
    Graphics::Program::FuncPtrs::SetUniformSint32(UniformIndex, Value);
}

void Game::Graphics::Program::SetUniformUint32(Sint32 UniformIndex,
                                               Uint32 Value) {
    Graphics::Program::FuncPtrs::SetUniformUint32(UniformIndex, Value);
}

void Game::Graphics::Program::SetUniformBool(Sint32 UniformIndex, bool Value) {
    Graphics::Program::FuncPtrs::SetUniformBool(UniformIndex, Value);
}

void Game::Graphics::Program::SetUniformFloat(Sint32 UniformIndex,
                                              float Value) {
    Graphics::Program::FuncPtrs::SetUniformFloat(UniformIndex, Value);
}

void Game::Graphics::Program::SetUniformIvec2(Sint32 UniformIndex,
                                              const Math::IVector2& Ivec2) {
    Graphics::Program::FuncPtrs::SetUniformIvec2(UniformIndex, Ivec2);
}

void Game::Graphics::Program::SetUniformUvec2(Sint32 UniformIndex,
                                              const Math::UVector2& Uvec2) {
    Graphics::Program::FuncPtrs::SetUniformUvec2(UniformIndex, Uvec2);
}

void Game::Graphics::Program::SetUniformBvec2(Sint32 UniformIndex,
                                              const Math::IVector2& Bvec2) {
    Graphics::Program::FuncPtrs::SetUniformBvec2(UniformIndex, Bvec2);
}

void Game::Graphics::Program::SetUniformFvec2(Sint32 UniformIndex,
                                              const Math::Vector2& Fvec2) {
    Graphics::Program::FuncPtrs::SetUniformFvec2(UniformIndex, Fvec2);
}

void Game::Graphics::Program::SetUniformIvec3(Sint32 UniformIndex,
                                              const Math::IVector3& Ivec3) {
    Graphics::Program::FuncPtrs::SetUniformIvec3(UniformIndex, Ivec3);
}

void Game::Graphics::Program::SetUniformUvec3(Sint32 UniformIndex,
                                              const Math::UVector3& Uvec3) {
    Graphics::Program::FuncPtrs::SetUniformUvec3(UniformIndex, Uvec3);
}

void Game::Graphics::Program::SetUniformBvec3(Sint32 UniformIndex,
                                              const Math::IVector3& Bvec3) {
    Graphics::Program::FuncPtrs::SetUniformBvec3(UniformIndex, Bvec3);
}

void Game::Graphics::Program::SetUniformFvec3(Sint32 UniformIndex,
                                              const Math::Vector3& Fvec3) {
    Graphics::Program::FuncPtrs::SetUniformFvec3(UniformIndex, Fvec3);
}

void Game::Graphics::Program::SetUniformIvec4(Sint32 UniformIndex,
                                              const Math::IVector4& Ivec4) {
    Graphics::Program::FuncPtrs::SetUniformIvec4(UniformIndex, Ivec4);
}

void Game::Graphics::Program::SetUniformUvec4(Sint32 UniformIndex,
                                              const Math::UVector4& Uvec4) {
    Graphics::Program::FuncPtrs::SetUniformUvec4(UniformIndex, Uvec4);
}

void Game::Graphics::Program::SetUniformBvec4(Sint32 UniformIndex,
                                              const Math::IVector4& Bvec4) {
    Graphics::Program::FuncPtrs::SetUniformBvec4(UniformIndex, Bvec4);
}

void Game::Graphics::Program::SetUniformFvec4(Sint32 UniformIndex,
                                              const Math::Vector4& Fvec4) {
    Graphics::Program::FuncPtrs::SetUniformFvec4(UniformIndex, Fvec4);
}

void Game::Graphics::Program::SetUniformMat2(Sint32 UniformIndex,
                                             const Math::Mat2& Mat2) {
    Graphics::Program::FuncPtrs::SetUniformMat2(UniformIndex, Mat2);
}

void Game::Graphics::Program::SetUniformMat3(Sint32 UniformIndex,
                                             const Math::Mat3& Mat3) {
    Graphics::Program::FuncPtrs::SetUniformMat3(UniformIndex, Mat3);
}

void Game::Graphics::Program::SetUniformMat4(Sint32 UniformIndex,
                                             const Math::Mat4& Mat4) {
    Graphics::Program::FuncPtrs::SetUniformMat4(UniformIndex, Mat4);
}