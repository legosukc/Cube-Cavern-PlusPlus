#include "Program.hpp"

// TODO: make the SetUniformBvec_ functions use a Bvec_ struct.

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
}