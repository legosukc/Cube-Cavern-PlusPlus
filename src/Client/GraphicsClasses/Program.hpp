#pragma once

#include <SDL3/SDL_opengl.h>
#include <SDL3/SDL_opengl_glext.h>
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
    extern PFNGLGETUNIFORMBLOCKINDEXPROC glGetUniformBlockIndex;
    extern PFNGLUNIFORMBLOCKBINDINGPROC glUniformBlockBinding;

    extern PFNGLATTACHSHADERPROC glAttachShader;
    extern PFNGLGETATTACHEDSHADERSPROC glGetAttachedShaders;
    extern PFNGLDETACHSHADERPROC glDetachShader;

    extern PFNGLLINKPROGRAMPROC glLinkProgram;
    extern PFNGLUSEPROGRAMPROC glUseProgram;

    extern PFNGLGETPROGRAMIVPROC glGetProgramiv;

    extern PFNGLUNIFORM1IPROC glUniform1i;
    extern PFNGLUNIFORM1UIPROC glUniform1ui;
    extern PFNGLUNIFORM1FPROC glUniform1f;

    extern PFNGLUNIFORM2IVPROC glUniform1iv;
    extern PFNGLUNIFORM2UIVPROC glUniform1uiv;
    extern PFNGLUNIFORM2FVPROC glUniform1fv;

    extern PFNGLUNIFORMMATRIX2FVPROC glUniformMatrix2fv;
    extern PFNGLUNIFORMMATRIX3FVPROC glUniformMatrix3fv;
    extern PFNGLUNIFORMMATRIX4FVPROC glUniformMatrix4fv;

    extern PFNGLCREATEPROGRAMPROC glCreateProgram;
    extern PFNGLDELETEPROGRAMPROC glDeleteProgram;
}

namespace Game::Graphics::Classes {

    class Program : public Game::Graphics::Classes::OpenGLClassBase {
       public:
        virtual ~Program();

        virtual GLint GetUniformLocation(const char* UniformName) const;

        virtual bool Link();
        virtual void Use();
    };

    class Program_OpenGL : public Program {
       public:
        virtual ~Program_OpenGL() override;

        virtual GLint GetUniformLocation(
            const char* UniformName) const override;

        virtual bool Link() override;
        virtual void Use() override;
    };
}

namespace Game::Graphics::Program::Wrappers::OpenGL {

    Game::Graphics::Classes::Program Create();

    inline void SetUniformSint32(Sint32 UniformIndex, Sint32 Value);
    inline void SetUniformUint32(Sint32 UniformIndex, Uint32 Value);
    inline void SetUniformBool(Sint32 UniformIndex, bool Value);
    inline void SetUniformFloat(Sint32 UniformIndex, float Value);

    template <class BoolVectorType>
    inline void SetUniformBvec_(Sint32 UniformIndex,
                                       const BoolVectorType BVector);

    template <class VectorType>
    inline void SetUniform_vec_(Sint32 UniformIndex,
                                       const VectorType& Vector);

    inline void SetUniformMat2(Sint32 UniformIndex,
                                      const Math::Mat2& Mat2);

    inline void SetUniformMat3(Sint32 UniformIndex,
                                      const Math::Mat3& Mat3);

    inline void SetUniformMat4(Sint32 UniformIndex,
                                      const Math::Mat4& Mat4);
}

// TODO: make the SetUniformBvec_ functions use a Bvec_ struct.

namespace Game::Graphics::Program {

    extern Game::Graphics::Classes::Program (*Create)();

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

    extern void (*SetUniformMat2)(Sint32 UniformIndex, const Math::Mat2& Mat2);
    extern void (*SetUniformMat3)(Sint32 UniformIndex, const Math::Mat3& Mat3);
    extern void (*SetUniformMat4)(Sint32 UniformIndex, const Math::Mat4& Mat4);

    inline void Init_OpenGL();
    inline void Init_Vulkan();
    inline void Init_Metal();
    inline void Init_DirectX11();
    inline void Init_DirectX12();
}