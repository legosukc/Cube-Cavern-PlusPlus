#ifndef GRAPHICS_PROGRAM_H
#define GRAPHICS_PROGRAM_H

// TODO: make the SetUniformBvec_ functions use a Bvec_ struct.

#include <iostream>

#include <SDL3/SDL_opengl.h>
#include <SDL3/SDL_opengl_glext.h>
#include <SDL3/SDL_video.h>

#include "../../MathClasses/Matrix.hpp"
#include "../../MathClasses/Vector2.hpp"
#include "../../MathClasses/Vector3.hpp"
#include "../../MathClasses/Vector4.hpp"

#include "BaseClassDefinitions.hpp"

namespace Game::Graphics::OpenGLFunctions {

    PFNGLCREATEPROGRAMPROC glCreateProgram = NULL;
    PFNGLDELETEPROGRAMPROC glDeleteProgram = NULL;

    PFNGLGETUNIFORMLOCATIONPROC glGetUniformLocation = NULL;
    PFNGLGETUNIFORMBLOCKINDEXPROC glGetUniformBlockIndex = NULL;
    PFNGLUNIFORMBLOCKBINDINGPROC glUniformBlockBinding = NULL;

    PFNGLATTACHSHADERPROC glAttachShader = NULL;
    PFNGLGETATTACHEDSHADERSPROC glGetAttachedShaders = NULL;
    PFNGLDETACHSHADERPROC glDetachShader = NULL;

    PFNGLLINKPROGRAMPROC glLinkProgram = NULL;
    PFNGLUSEPROGRAMPROC glUseProgram = NULL;

    PFNGLGETPROGRAMIVPROC glGetProgramiv = NULL;

    PFNGLUNIFORM1IPROC glUniform1i = NULL;
    PFNGLUNIFORM1UIPROC glUniform1ui = NULL;
    PFNGLUNIFORM1FPROC glUniform1f = NULL;

    // PFNGLUNIFORM2IVPROC glUniform1iv = NULL;
    // PFNGLUNIFORM2UIVPROC glUniform1uiv = NULL;
    // PFNGLUNIFORM2FVPROC glUniform1fv = NULL;

    PFNGLUNIFORM2IVPROC glUniform2iv = NULL;
    PFNGLUNIFORM2UIVPROC glUniform2uiv = NULL;
    PFNGLUNIFORM2FVPROC glUniform2fv = NULL;

    PFNGLUNIFORM2IVPROC glUniform3iv = NULL;
    PFNGLUNIFORM2UIVPROC glUniform3uiv = NULL;
    PFNGLUNIFORM2FVPROC glUniform3fv = NULL;

    PFNGLUNIFORM2IVPROC glUniform4iv = NULL;
    PFNGLUNIFORM2UIVPROC glUniform4uiv = NULL;
    PFNGLUNIFORM2FVPROC glUniform4fv = NULL;

    PFNGLUNIFORMMATRIX2FVPROC glUniformMatrix2fv = NULL;
    PFNGLUNIFORMMATRIX3FVPROC glUniformMatrix3fv = NULL;
    PFNGLUNIFORMMATRIX4FVPROC glUniformMatrix4fv = NULL;
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

    Game::Graphics::Classes::Program* Create();

    inline void SetUniformSint32(Sint32 UniformIndex, Sint32 Value);
    inline void SetUniformUint32(Sint32 UniformIndex, Uint32 Value);
    inline void SetUniformBool(Sint32 UniformIndex, bool Value);
    inline void SetUniformFloat(Sint32 UniformIndex, float Value);

    template <class BoolVectorType>
    inline void SetUniformBvec_(Sint32 UniformIndex,
                                const BoolVectorType BVector);

    template <class VectorType>
    inline void SetUniform_vec2(Sint32 UniformIndex, const VectorType& Vector);

    template <class VectorType>
    inline void SetUniform_vec3(Sint32 UniformIndex, const VectorType& Vector);

    template <class VectorType>
    inline void SetUniform_vec4(Sint32 UniformIndex, const VectorType& Vector);

    inline void SetUniformMat2(Sint32 UniformIndex, const Math::Mat2& Mat2);

    inline void SetUniformMat3(Sint32 UniformIndex, const Math::Mat3& Mat3);

    inline void SetUniformMat4(Sint32 UniformIndex, const Math::Mat4& Mat4);
}

// TODO: make the SetUniformBvec_ functions use a Bvec_ struct.

namespace Game::Graphics::Program {

    namespace FuncPtrs {
        Game::Graphics::Classes::Program* (*Create)() = NULL;

        void (*SetUniformSint32)(Sint32 UniformIndex, Sint32 Value) = NULL;
        void (*SetUniformUint32)(Sint32 UniformIndex, Uint32 Value) = NULL;
        void (*SetUniformBool)(Sint32 UniformIndex, bool Value) = NULL;
        void (*SetUniformFloat)(Sint32 UniformIndex, float Value) = NULL;

        void (*SetUniformIvec2)(Sint32 UniformIndex,
                                const Math::IVector2& Ivec2) = NULL;
        void (*SetUniformUvec2)(Sint32 UniformIndex,
                                const Math::UVector2& Uvec2) = NULL;
        void (*SetUniformBvec2)(Sint32 UniformIndex,
                                const Math::IVector2& Bvec2) = NULL;
        void (*SetUniformFvec2)(Sint32 UniformIndex,
                                const Math::Vector2& Fvec2) = NULL;

        void (*SetUniformIvec3)(Sint32 UniformIndex,
                                const Math::IVector3& Ivec3) = NULL;
        void (*SetUniformUvec3)(Sint32 UniformIndex,
                                const Math::UVector3& Uvec3) = NULL;
        void (*SetUniformBvec3)(Sint32 UniformIndex,
                                const Math::IVector3& Bvec3) = NULL;
        void (*SetUniformFvec3)(Sint32 UniformIndex,
                                const Math::Vector3& Fvec3) = NULL;

        void (*SetUniformIvec4)(Sint32 UniformIndex,
                                const Math::IVector4& Ivec4) = NULL;
        void (*SetUniformUvec4)(Sint32 UniformIndex,
                                const Math::UVector4& Uvec4) = NULL;
        void (*SetUniformBvec4)(Sint32 UniformIndex,
                                const Math::IVector4& Bvec4) = NULL;
        void (*SetUniformFvec4)(Sint32 UniformIndex,
                                const Math::Vector4& Fvec4) = NULL;

        void (*SetUniformMat2)(Sint32 UniformIndex,
                               const Math::Mat2& Mat2) = NULL;
        void (*SetUniformMat3)(Sint32 UniformIndex,
                               const Math::Mat3& Mat3) = NULL;
        void (*SetUniformMat4)(Sint32 UniformIndex,
                               const Math::Mat4& Mat4) = NULL;
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

Game::Graphics::Classes::Program::~Program() = default;

GLint Game::Graphics::Classes::Program::GetUniformLocation(
    const char* UniformName) const {
    return -1;
}

bool Game::Graphics::Classes::Program::Link() {
    return false;
}
void Game::Graphics::Classes::Program::Use() {}

Game::Graphics::Classes::Program_OpenGL::~Program_OpenGL() {
    if (this->GLObject == 0) {
        return;
    }
    Game::Graphics::OpenGLFunctions::glDeleteProgram(this->GLObject);
}

GLint Game::Graphics::Classes::Program_OpenGL::GetUniformLocation(
    const char* UniformName) const {
    return Game::Graphics::OpenGLFunctions::glGetUniformLocation(this->GLObject,
                                                                 UniformName);
}

bool Game::Graphics::Classes::Program_OpenGL::Link() {
    Game::Graphics::OpenGLFunctions::glLinkProgram(this->GLObject);
    return true;
}

void Game::Graphics::Classes::Program_OpenGL::Use() {
    Game::Graphics::OpenGLFunctions::glUseProgram(this->GLObject);
}

Game::Graphics::Classes::Program*
    Game::Graphics::Program::Wrappers::OpenGL::Create() {
    Game::Graphics::Classes::Program_OpenGL* NewProgram =
        new Game::Graphics::Classes::Program_OpenGL;
    NewProgram->GLObject = Game::Graphics::OpenGLFunctions::glCreateProgram();

    return NewProgram;
}

void Game::Graphics::Program::Wrappers::OpenGL::SetUniformSint32(
    Sint32 UniformIndex,
    Sint32 Value) {
    Game::Graphics::OpenGLFunctions::glUniform1i(UniformIndex, Value);
}

void Game::Graphics::Program::Wrappers::OpenGL::SetUniformUint32(
    Sint32 UniformIndex,
    Uint32 Value) {
    Game::Graphics::OpenGLFunctions::glUniform1ui(UniformIndex, Value);
}

void Game::Graphics::Program::Wrappers::OpenGL::SetUniformBool(
    Sint32 UniformIndex,
    bool Value) {
    Game::Graphics::OpenGLFunctions::glUniform1ui(UniformIndex, Value);
}

void Game::Graphics::Program::Wrappers::OpenGL::SetUniformFloat(
    Sint32 UniformIndex,
    float Value) {
    Game::Graphics::OpenGLFunctions::glUniform1f(UniformIndex, Value);
}

template <class BoolVectorType>
void Game::Graphics::Program::Wrappers::OpenGL::SetUniformBvec_(
    Sint32 UniformIndex,
    const BoolVectorType BVector) {
    if constexpr (BoolVectorType::ComponentCount == 2) {
        Game::Graphics::OpenGLFunctions::glUniform2iv(
            UniformIndex, 1, &BVector);
    } else if constexpr (BoolVectorType::ComponentCount == 3) {
        Game::Graphics::OpenGLFunctions::glUniform3iv(
            UniformIndex, 1, &BVector);
    } else if constexpr (BoolVectorType::ComponentCount == 4) {
        Game::Graphics::OpenGLFunctions::glUniform4iv(
            UniformIndex, 1, &BVector);
    }
}

template <class VectorType>
void Game::Graphics::Program::Wrappers::OpenGL::SetUniform_vec2(
    Sint32 UniformIndex,
    const VectorType& Vector) {
    using namespace Game::Graphics::OpenGLFunctions;

    if constexpr (std::is_floating_point_v<
                      typename VectorType::ComponentType>) {
        Game::Graphics::OpenGLFunctions::glUniform2fv(
            UniformIndex, 1, reinterpret_cast<const float*>(&Vector));

    } else if constexpr (std::is_signed_v<typename VectorType::ComponentType>) {
        Game::Graphics::OpenGLFunctions::glUniform2iv(UniformIndex, 1,
                                                      &Vector.X);

    } else if constexpr (std::is_unsigned_v<
                             typename VectorType::ComponentType>) {
        Game::Graphics::OpenGLFunctions::glUniform2uiv(UniformIndex, 1,
                                                       &Vector.X);
    }
}

template <class VectorType>
void Game::Graphics::Program::Wrappers::OpenGL::SetUniform_vec3(
    Sint32 UniformIndex,
    const VectorType& Vector) {
    using namespace Game::Graphics::OpenGLFunctions;

    if constexpr (std::is_floating_point_v<
                      typename VectorType::ComponentType>) {
        Game::Graphics::OpenGLFunctions::glUniform3fv(
            UniformIndex, 1,  // VectorType::ComponentCount,
            reinterpret_cast<const float*>(&Vector));

    } else if constexpr (std::is_signed_v<typename VectorType::ComponentType>) {
        Game::Graphics::OpenGLFunctions::glUniform3iv(UniformIndex, 1,
                                                      &Vector.X);

    } else if constexpr (std::is_unsigned_v<
                             typename VectorType::ComponentType>) {
        Game::Graphics::OpenGLFunctions::glUniform3uiv(UniformIndex, 1,
                                                       &Vector.X);
    }
}

template <class VectorType>
void Game::Graphics::Program::Wrappers::OpenGL::SetUniform_vec4(
    Sint32 UniformIndex,
    const VectorType& Vector) {
    using namespace Game::Graphics::OpenGLFunctions;

    if constexpr (std::is_floating_point_v<
                      typename VectorType::ComponentType>) {
        Game::Graphics::OpenGLFunctions::glUniform4fv(
            UniformIndex, 1, reinterpret_cast<const float*>(&Vector));

    } else if constexpr (std::is_signed_v<typename VectorType::ComponentType>) {
        Game::Graphics::OpenGLFunctions::glUniform4iv(UniformIndex, 1,
                                                      &Vector.X);

    } else if constexpr (std::is_unsigned_v<
                             typename VectorType::ComponentType>) {
        Game::Graphics::OpenGLFunctions::glUniform4uiv(UniformIndex, 1,
                                                       &Vector.X);
    }
}

void Game::Graphics::Program::Wrappers::OpenGL::SetUniformMat2(
    Sint32 UniformIndex,
    const Math::Mat2& Mat2) {
    Game::Graphics::OpenGLFunctions::glUniformMatrix2fv(UniformIndex, 1,
                                                        GL_FALSE, &Mat2.R0.X);
}

void Game::Graphics::Program::Wrappers::OpenGL::SetUniformMat3(
    Sint32 UniformIndex,
    const Math::Mat3& Mat3) {
    Game::Graphics::OpenGLFunctions::glUniformMatrix3fv(UniformIndex, 1,
                                                        GL_FALSE, &Mat3.R0.X);
}

void Game::Graphics::Program::Wrappers::OpenGL::SetUniformMat4(
    Sint32 UniformIndex,
    const Math::Mat4& Mat4) {
    Game::Graphics::OpenGLFunctions::glUniformMatrix4fv(UniformIndex, 1,
                                                        GL_FALSE, &Mat4.R0.X);
}

void Game::Graphics::Program::Init_OpenGL() {
    LOAD_OPENGL_FUNCTION(glCreateProgram);
    LOAD_OPENGL_FUNCTION(glDeleteProgram);

    LOAD_OPENGL_FUNCTION(glGetUniformLocation);
    LOAD_OPENGL_FUNCTION(glGetUniformBlockIndex);

    LOAD_OPENGL_FUNCTION(glUniformBlockBinding);

    LOAD_OPENGL_FUNCTION(glAttachShader);
    LOAD_OPENGL_FUNCTION(glGetAttachedShaders);
    LOAD_OPENGL_FUNCTION(glDetachShader);

    LOAD_OPENGL_FUNCTION(glLinkProgram);
    LOAD_OPENGL_FUNCTION(glUseProgram);

    LOAD_OPENGL_FUNCTION(glGetProgramiv);

    LOAD_OPENGL_FUNCTION(glUniform1i);
    LOAD_OPENGL_FUNCTION(glUniform1ui);
    LOAD_OPENGL_FUNCTION(glUniform1f);

    LOAD_OPENGL_FUNCTION(glUniform2iv);
    LOAD_OPENGL_FUNCTION(glUniform2uiv);
    LOAD_OPENGL_FUNCTION(glUniform2fv);

    LOAD_OPENGL_FUNCTION(glUniform3iv);
    LOAD_OPENGL_FUNCTION(glUniform3uiv);
    LOAD_OPENGL_FUNCTION(glUniform3fv);

    LOAD_OPENGL_FUNCTION(glUniform4iv);
    LOAD_OPENGL_FUNCTION(glUniform4uiv);
    LOAD_OPENGL_FUNCTION(glUniform4fv);

    LOAD_OPENGL_FUNCTION(glUniformMatrix2fv);
    LOAD_OPENGL_FUNCTION(glUniformMatrix3fv);
    LOAD_OPENGL_FUNCTION(glUniformMatrix4fv);

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

inline Game::Graphics::Classes::Program* Game::Graphics::Program::Create() {
    return Graphics::Program::FuncPtrs::Create();
}

inline void Game::Graphics::Program::SetUniformSint32(Sint32 UniformIndex,
                                                      Sint32 Value) {
    Graphics::Program::FuncPtrs::SetUniformSint32(UniformIndex, Value);
}

inline void Game::Graphics::Program::SetUniformUint32(Sint32 UniformIndex,
                                                      Uint32 Value) {
    Graphics::Program::FuncPtrs::SetUniformUint32(UniformIndex, Value);
}

inline void Game::Graphics::Program::SetUniformBool(Sint32 UniformIndex,
                                                    bool Value) {
    Graphics::Program::FuncPtrs::SetUniformBool(UniformIndex, Value);
}

inline void Game::Graphics::Program::SetUniformFloat(Sint32 UniformIndex,
                                                     float Value) {
    Graphics::Program::FuncPtrs::SetUniformFloat(UniformIndex, Value);
}

inline void Game::Graphics::Program::SetUniformIvec2(
    Sint32 UniformIndex,
    const Math::IVector2& Ivec2) {
    Graphics::Program::FuncPtrs::SetUniformIvec2(UniformIndex, Ivec2);
}

inline void Game::Graphics::Program::SetUniformUvec2(
    Sint32 UniformIndex,
    const Math::UVector2& Uvec2) {
    Graphics::Program::FuncPtrs::SetUniformUvec2(UniformIndex, Uvec2);
}

inline void Game::Graphics::Program::SetUniformBvec2(
    Sint32 UniformIndex,
    const Math::IVector2& Bvec2) {
    Graphics::Program::FuncPtrs::SetUniformBvec2(UniformIndex, Bvec2);
}

inline void Game::Graphics::Program::SetUniformFvec2(
    Sint32 UniformIndex,
    const Math::Vector2& Fvec2) {
    Graphics::Program::FuncPtrs::SetUniformFvec2(UniformIndex, Fvec2);
}

inline void Game::Graphics::Program::SetUniformIvec3(
    Sint32 UniformIndex,
    const Math::IVector3& Ivec3) {
    Graphics::Program::FuncPtrs::SetUniformIvec3(UniformIndex, Ivec3);
}

inline void Game::Graphics::Program::SetUniformUvec3(
    Sint32 UniformIndex,
    const Math::UVector3& Uvec3) {
    Graphics::Program::FuncPtrs::SetUniformUvec3(UniformIndex, Uvec3);
}

inline void Game::Graphics::Program::SetUniformBvec3(
    Sint32 UniformIndex,
    const Math::IVector3& Bvec3) {
    Graphics::Program::FuncPtrs::SetUniformBvec3(UniformIndex, Bvec3);
}

inline void Game::Graphics::Program::SetUniformFvec3(
    Sint32 UniformIndex,
    const Math::Vector3& Fvec3) {
    Graphics::Program::FuncPtrs::SetUniformFvec3(UniformIndex, Fvec3);
}

inline void Game::Graphics::Program::SetUniformIvec4(
    Sint32 UniformIndex,
    const Math::IVector4& Ivec4) {
    Graphics::Program::FuncPtrs::SetUniformIvec4(UniformIndex, Ivec4);
}

inline void Game::Graphics::Program::SetUniformUvec4(
    Sint32 UniformIndex,
    const Math::UVector4& Uvec4) {
    Graphics::Program::FuncPtrs::SetUniformUvec4(UniformIndex, Uvec4);
}

inline void Game::Graphics::Program::SetUniformBvec4(
    Sint32 UniformIndex,
    const Math::IVector4& Bvec4) {
    Graphics::Program::FuncPtrs::SetUniformBvec4(UniformIndex, Bvec4);
}

inline void Game::Graphics::Program::SetUniformFvec4(
    Sint32 UniformIndex,
    const Math::Vector4& Fvec4) {
    Graphics::Program::FuncPtrs::SetUniformFvec4(UniformIndex, Fvec4);
}

inline void Game::Graphics::Program::SetUniformMat2(Sint32 UniformIndex,
                                                    const Math::Mat2& Mat2) {
    Graphics::Program::FuncPtrs::SetUniformMat2(UniformIndex, Mat2);
}

inline void Game::Graphics::Program::SetUniformMat3(Sint32 UniformIndex,
                                                    const Math::Mat3& Mat3) {
    Graphics::Program::FuncPtrs::SetUniformMat3(UniformIndex, Mat3);
}

inline void Game::Graphics::Program::SetUniformMat4(Sint32 UniformIndex,
                                                    const Math::Mat4& Mat4) {
    Graphics::Program::FuncPtrs::SetUniformMat4(UniformIndex, Mat4);
}

#endif