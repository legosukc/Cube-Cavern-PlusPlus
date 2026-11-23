#pragma once

// TODO: make the SetUniformBvec_ functions use a Bvec_ struct.

#include "../../../define.h"

#ifdef SDL_PLATFORM_VITA
#include <GLES2/gl2.h>
#include "../../../GLES2_typedefs.h"
#else
#include <SDL3/SDL_opengl.h>
#include <SDL3/SDL_opengl_glext.h>
#endif

#include <SDL3/SDL_video.h>

#include "../../../MathClasses/Matrix.hpp"
#include "../../../MathClasses/Vector2.hpp"
#include "../../../MathClasses/Vector3.hpp"
#include "../../../MathClasses/Vector4.hpp"

#include "Program/Base_Program.hpp"

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

    inline Game::Graphics::Classes::Program* Create() {
        return Graphics::Program::FuncPtrs::Create();
    }

    inline void SetUniformSint32(Sint32 UniformIndex, Sint32 Value) {
        Graphics::Program::FuncPtrs::SetUniformSint32(UniformIndex, Value);
    }

    inline void SetUniformUint32(Sint32 UniformIndex, Uint32 Value) {
        Graphics::Program::FuncPtrs::SetUniformUint32(UniformIndex, Value);
    }

    inline void SetUniformBool(Sint32 UniformIndex, bool Value) {
        Graphics::Program::FuncPtrs::SetUniformBool(UniformIndex, Value);
    }

    inline void SetUniformFloat(Sint32 UniformIndex, float Value) {
        Graphics::Program::FuncPtrs::SetUniformFloat(UniformIndex, Value);
    }

    inline void SetUniformIvec2(Sint32 UniformIndex,
                                const Math::IVector2& Ivec2) {
        Graphics::Program::FuncPtrs::SetUniformIvec2(UniformIndex, Ivec2);
    }

    inline void SetUniformUvec2(Sint32 UniformIndex,
                                const Math::UVector2& Uvec2) {
        Graphics::Program::FuncPtrs::SetUniformUvec2(UniformIndex, Uvec2);
    }

    inline void SetUniformBvec2(Sint32 UniformIndex,
                                const Math::IVector2& Bvec2) {
        Graphics::Program::FuncPtrs::SetUniformBvec2(UniformIndex, Bvec2);
    }

    inline void SetUniformFvec2(Sint32 UniformIndex,
                                const Math::Vector2& Fvec2) {
        Graphics::Program::FuncPtrs::SetUniformFvec2(UniformIndex, Fvec2);
    }

    inline void SetUniformIvec3(Sint32 UniformIndex,
                                const Math::IVector3& Ivec3) {
        Graphics::Program::FuncPtrs::SetUniformIvec3(UniformIndex, Ivec3);
    }

    inline void SetUniformUvec3(Sint32 UniformIndex,
                                const Math::UVector3& Uvec3) {
        Graphics::Program::FuncPtrs::SetUniformUvec3(UniformIndex, Uvec3);
    }

    inline void SetUniformBvec3(Sint32 UniformIndex,
                                const Math::IVector3& Bvec3) {
        Graphics::Program::FuncPtrs::SetUniformBvec3(UniformIndex, Bvec3);
    }

    inline void SetUniformFvec3(Sint32 UniformIndex,
                                const Math::Vector3& Fvec3) {
        Graphics::Program::FuncPtrs::SetUniformFvec3(UniformIndex, Fvec3);
    }

    inline void SetUniformIvec4(Sint32 UniformIndex,
                                const Math::IVector4& Ivec4) {
        Graphics::Program::FuncPtrs::SetUniformIvec4(UniformIndex, Ivec4);
    }

    inline void SetUniformUvec4(Sint32 UniformIndex,
                                const Math::UVector4& Uvec4) {
        Graphics::Program::FuncPtrs::SetUniformUvec4(UniformIndex, Uvec4);
    }

    inline void SetUniformBvec4(Sint32 UniformIndex,
                                const Math::IVector4& Bvec4) {
        Graphics::Program::FuncPtrs::SetUniformBvec4(UniformIndex, Bvec4);
    }

    inline void SetUniformFvec4(Sint32 UniformIndex,
                                const Math::Vector4& Fvec4) {
        Graphics::Program::FuncPtrs::SetUniformFvec4(UniformIndex, Fvec4);
    }

    inline void SetUniformMat2(Sint32 UniformIndex, const Math::Mat2& Mat2) {
        Graphics::Program::FuncPtrs::SetUniformMat2(UniformIndex, Mat2);
    }

    inline void SetUniformMat3(Sint32 UniformIndex, const Math::Mat3& Mat3) {
        Graphics::Program::FuncPtrs::SetUniformMat3(UniformIndex, Mat3);
    }

    inline void SetUniformMat4(Sint32 UniformIndex, const Math::Mat4& Mat4) {
        Graphics::Program::FuncPtrs::SetUniformMat4(UniformIndex, Mat4);
    }
}