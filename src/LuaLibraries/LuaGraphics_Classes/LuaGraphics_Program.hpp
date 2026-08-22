#pragma once

#include "../../define.h"

#include "LuaGraphics_GLObjectBase.hpp"
#include "LuaGraphics_Shader.hpp"

#include <SDL3/SDL_joystick.h>
#include <SDL3/SDL_opengl.h>
#include <SDL3/SDL_opengl_glext.h>

#ifdef BUILD_CLIENT
#include "../../Client/GraphicsClasses/Program.hpp"
#endif

#include <SDL3/SDL_opengl.h>
#include <SDL3/SDL_opengl_glext.h>

#include "../../../include/VM/lua.h"
#include "../../../include/VM/lualib.h"

#include "../../MathClasses/Vector3.hpp"

namespace Game::Lua::CLibraries::Graphics {

    namespace Classes {
        struct Program
            : Game::Lua::CLibraries::Graphics::Classes::GLObjectBase {
            static int AttachShader(lua_State* State);
            static int DetachShader(lua_State* State);
            static int DetachAllShaders(lua_State* State);

            static int Link(lua_State* State);
            static int Use(lua_State* State);

            static int GetUniformIndex(lua_State* State);

            static int SetUniformBlockBinding(lua_State* State);
            static int GetUniformBlockIndex(lua_State* State);

            static int __gc(lua_State* State);
        };
    }

    namespace Program {
        inline void Init(lua_State* State,
                         LuaHelper::StackTableReference& GraphicsTable);

        // safe to directly c call
        int __new(lua_State* State);

        int SetUniformSint32(lua_State* State);
        int SetUniformUint32(lua_State* State);
        int SetUniformFloat(lua_State* State);

        int SetUniformIvec2(lua_State* State);
        int SetUniformUvec2(lua_State* State);
        int SetUniformBvec2(lua_State* State);
        int SetUniformFvec2(lua_State* State);

        int SetUniformIvec3(lua_State* State);
        int SetUniformUvec3(lua_State* State);
        int SetUniformBvec3(lua_State* State);
        int SetUniformFvec3(lua_State* State);

        int SetUniformIvec4(lua_State* State);
        int SetUniformUvec4(lua_State* State);
        int SetUniformBvec4(lua_State* State);
        int SetUniformFvec4(lua_State* State);

        int SetUniformMat2(lua_State* State);
        int SetUniformMat3(lua_State* State);
        int SetUniformMat4(lua_State* State);
    }
}
/*
namespace Game::Lua::CLibraries::Graphics::Program {

    int SetUniformSint32(lua_State* State);
    int SetUniformUint32(lua_State* State);
    int SetUniformFloat(lua_State* State);

    int SetUniformIvec2(lua_State* State);
    int SetUniformUvec2(lua_State* State);
    int SetUniformBvec2(lua_State* State);
    int SetUniformFvec2(lua_State* State);

    int SetUniformIvec3(lua_State* State);
    int SetUniformUvec3(lua_State* State);
    int SetUniformBvec3(lua_State* State);
    int SetUniformFvec3(lua_State* State);

    int SetUniformIvec4(lua_State* State);
    int SetUniformUvec4(lua_State* State);
    int SetUniformBvec4(lua_State* State);
    int SetUniformFvec4(lua_State* State);

    int SetUniformMat2(lua_State* State);
    int SetUniformMat3(lua_State* State);
    int SetUniformMat4(lua_State* State);
}
*/

// implementation

void Game::Lua::CLibraries::Graphics::Program::Init(
    lua_State* State,
    LuaHelper::StackTableReference& GraphicsTable) {
    LuaHelper::StackTableReference ProgramMetatable(State, "Program");

    ProgramMetatable.SetKeyClosure(State, Classes::Program::__gc, "__gc");

    ProgramMetatable.PushReference(State);
    lua_setfield(State, ProgramMetatable.GetStackIndex(), "__index");

    ProgramMetatable.SetKeyClosure(State, Classes::Program::GetUniformIndex,
                                   "GetUniformIndex");

    ProgramMetatable.SetKeyClosure(State,
                                   Classes::Program::SetUniformBlockBinding,
                                   "SetUniformBlockBinding");
    ProgramMetatable.SetKeyClosure(
        State, Classes::Program::GetUniformBlockIndex, "GetUniformBlockIndex");

    ProgramMetatable.SetKeyClosure(State, Classes::Program::AttachShader,
                                   "AttachShader");
    ProgramMetatable.SetKeyClosure(State, Classes::Program::DetachShader,
                                   "DetachShader");
    ProgramMetatable.SetKeyClosure(State, Classes::Program::DetachAllShaders,
                                   "DetachAllShaders");

    ProgramMetatable.SetKeyClosure(State, Classes::Program::Link, "Link");

    ProgramMetatable.PushReference(State);
    ProgramMetatable.SetKeyClosure(State, Classes::Program::Use, "Use", 1);

    lua_settop(State, ProgramMetatable.GetStackIndex() - 1);

    LuaHelper::StackTableReference ProgramTable(State, 0,
                                                17);  // OpenGL.Program

    ProgramTable.SetKeyClosure(State, CLibraries::Graphics::Program::__new,
                               "new");

    ProgramTable.SetKeyClosure(State,
                               CLibraries::Graphics::Program::SetUniformSint32,
                               "SetUniformSint32");
    ProgramTable.SetKeyClosure(State,
                               CLibraries::Graphics::Program::SetUniformUint32,
                               "SetUniformUint32");

    // TODO: change this to use a SetUniformBool thing
    ProgramTable.SetKeyClosure(State,
                               CLibraries::Graphics::Program::SetUniformSint32,
                               "SetUniformBool");
    ProgramTable.SetKeyClosure(State,
                               CLibraries::Graphics::Program::SetUniformFloat,
                               "SetUniformFloat");

    ProgramTable.SetKeyClosure(State,
                               CLibraries::Graphics::Program::SetUniformIvec2,
                               "SetUniformIvec2");
    ProgramTable.SetKeyClosure(State,
                               CLibraries::Graphics::Program::SetUniformUvec2,
                               "SetUniformUvec2");
    ProgramTable.SetKeyClosure(State,
                               CLibraries::Graphics::Program::SetUniformBvec2,
                               "SetUniformBvec2");
    ProgramTable.SetKeyClosure(State,
                               CLibraries::Graphics::Program::SetUniformFvec2,
                               "SetUniformVec2");

    ProgramTable.SetKeyClosure(State,
                               CLibraries::Graphics::Program::SetUniformIvec3,
                               "SetUniformIvec3");
    ProgramTable.SetKeyClosure(State,
                               CLibraries::Graphics::Program::SetUniformUvec3,
                               "SetUniformUvec3");
    ProgramTable.SetKeyClosure(State,
                               CLibraries::Graphics::Program::SetUniformBvec3,
                               "SetUniformBvec3");
    ProgramTable.SetKeyClosure(State,
                               CLibraries::Graphics::Program::SetUniformFvec3,
                               "SetUniformVec3");

    ProgramTable.SetKeyClosure(State,
                               CLibraries::Graphics::Program::SetUniformIvec4,
                               "SetUniformIvec4");
    ProgramTable.SetKeyClosure(State,
                               CLibraries::Graphics::Program::SetUniformUvec4,
                               "SetUniformUvec4");
    ProgramTable.SetKeyClosure(State,
                               CLibraries::Graphics::Program::SetUniformBvec4,
                               "SetUniformBvec4");
    ProgramTable.SetKeyClosure(State,
                               CLibraries::Graphics::Program::SetUniformFvec4,
                               "SetUniformVec4");

    ProgramTable.SetKeyClosure(
        State, CLibraries::Graphics::Program::SetUniformMat2, "SetUniformMat2");
    ProgramTable.SetKeyClosure(
        State, CLibraries::Graphics::Program::SetUniformMat3, "SetUniformMat3");
    ProgramTable.SetKeyClosure(
        State, CLibraries::Graphics::Program::SetUniformMat4, "SetUniformMat4");

    lua_setfield(State, GraphicsTable.GetStackIndex(), "Program");
}

int Game::Lua::CLibraries::Graphics::Program::__new(lua_State* State) {
    CLibraries::Graphics::Classes::Program* ProgramUD =
        static_cast<CLibraries::Graphics::Classes::Program*>(
            lua_newuserdata(State, sizeof(Classes::Program)));
    ProgramUD->GLObject = Game::Graphics::OpenGLFunctions::glCreateProgram();

    luaL_getmetatable(State, "Program");
    lua_setmetatable(State, -2);
    return 1;
}

int Game::Lua::CLibraries::Graphics::Program::SetUniformSint32(
    lua_State* State) {
    Game::Graphics::Program::SetUniformSint32(
        static_cast<Sint32>(luaL_checkinteger(State, 1)),
        static_cast<Sint32>(luaL_checkinteger(State, 2)));
    return 0;
}

int Game::Lua::CLibraries::Graphics::Program::SetUniformUint32(
    lua_State* State) {
    Game::Graphics::Program::SetUniformUint32(
        static_cast<Sint32>(luaL_checkinteger(State, 1)),
        static_cast<Uint32>(luaL_checkinteger(State, 2)));
    return 0;
}

int Game::Lua::CLibraries::Graphics::Program::SetUniformFloat(
    lua_State* State) {
    Game::Graphics::Program::SetUniformFloat(
        static_cast<Sint32>(luaL_checkinteger(State, 1)),
        static_cast<float>(luaL_checknumber(State, 2)));
    return 0;
}

int Game::Lua::CLibraries::Graphics::Program::SetUniformIvec2(
    lua_State* State) {
    if (const void* VecUD = LuaHelper::TestMetatable(State, 2, "IVector2");
        VecUD == NULL) {
        Game::Graphics::Program::SetUniformIvec2(
            static_cast<Sint32>(luaL_checkinteger(State, 1)),
            Math::IVector2(static_cast<Sint32>(luaL_checkinteger(State, 2)),
                           static_cast<Sint32>(luaL_checkinteger(State, 3))));
    } else {
        Game::Graphics::Program::SetUniformIvec2(
            static_cast<Sint32>(luaL_checkinteger(State, 1)),
            *static_cast<const Math::IVector2*>(VecUD));
    }

    return 0;
}

int Game::Lua::CLibraries::Graphics::Program::SetUniformUvec2(
    lua_State* State) {
    if (const void* VecUD = LuaHelper::TestMetatable(State, 2, "UVector2");
        VecUD == NULL) {
        Game::Graphics::Program::SetUniformUvec2(
            static_cast<Sint32>(luaL_checkinteger(State, 1)),
            Math::UVector2(static_cast<Uint32>(luaL_checkinteger(State, 2)),
                           static_cast<Uint32>(luaL_checkinteger(State, 3))));
    } else {
        Game::Graphics::Program::SetUniformUvec2(
            static_cast<Sint32>(luaL_checkinteger(State, 1)),
            *static_cast<const Math::UVector2*>(VecUD));
    }

    return 0;
}

int Game::Lua::CLibraries::Graphics::Program::SetUniformBvec2(
    lua_State* State) {
    Game::Graphics::Program::SetUniformIvec2(
        static_cast<Sint32>(luaL_checkinteger(State, 1)),
        Math::IVector2(static_cast<Sint32>(luaL_checkinteger(State, 2)),
                       static_cast<Sint32>(luaL_checkinteger(State, 3))));

    return 0;
}

int Game::Lua::CLibraries::Graphics::Program::SetUniformFvec2(
    lua_State* State) {
    if (const void* VecUD = LuaHelper::TestMetatable(State, 2, "Vector2");
        VecUD == NULL) {
        Game::Graphics::Program::SetUniformFvec2(
            static_cast<GLint>(luaL_checkinteger(State, 1)),
            Math::Vector2(static_cast<float>(luaL_checknumber(State, 2)),
                          static_cast<float>(luaL_checknumber(State, 3))));
    } else {
        Game::Graphics::Program::SetUniformFvec2(
            static_cast<GLint>(luaL_checkinteger(State, 1)),
            *static_cast<const Math::Vector2*>(VecUD));
    }

    return 0;
}

int Game::Lua::CLibraries::Graphics::Program::SetUniformIvec3(
    lua_State* State) {
    if (const void* VecUD = LuaHelper::TestMetatable(State, 2, "IVector3");
        VecUD == NULL) {
        Game::Graphics::Program::SetUniformIvec3(
            static_cast<Sint32>(luaL_checkinteger(State, 1)),
            Math::IVector3(static_cast<Sint32>(luaL_checkinteger(State, 2)),
                           static_cast<Sint32>(luaL_checkinteger(State, 3)),
                           static_cast<Sint32>(luaL_checkinteger(State, 4))));
    } else {
        Game::Graphics::Program::SetUniformIvec3(
            static_cast<Sint32>(luaL_checkinteger(State, 1)),
            *static_cast<const Math::IVector3*>(VecUD));
    }
    return 0;
}

int Game::Lua::CLibraries::Graphics::Program::SetUniformUvec3(
    lua_State* State) {
    if (const void* VecUD = LuaHelper::TestMetatable(State, 2, "UVector3");
        VecUD == NULL) {
        Game::Graphics::Program::SetUniformUvec3(
            static_cast<Sint32>(luaL_checkinteger(State, 1)),
            Math::UVector3(static_cast<Uint32>(luaL_checkinteger(State, 2)),
                           static_cast<Uint32>(luaL_checkinteger(State, 3)),
                           static_cast<Uint32>(luaL_checkinteger(State, 4))));
    } else {
        Game::Graphics::Program::SetUniformUvec3(
            static_cast<Sint32>(luaL_checkinteger(State, 1)),
            *static_cast<const Math::UVector3*>(VecUD));
    }
    return 0;
}

int Game::Lua::CLibraries::Graphics::Program::SetUniformBvec3(
    lua_State* State) {
    Game::Graphics::Program::SetUniformBvec3(
        static_cast<Sint32>(luaL_checkinteger(State, 1)),
        Math::IVector3(static_cast<GLint>(luaL_checkinteger(State, 2)),
                       static_cast<GLint>(luaL_checkinteger(State, 3)),
                       static_cast<GLint>(luaL_checkinteger(State, 4))));
    return 0;
}

int Game::Lua::CLibraries::Graphics::Program::SetUniformFvec3(
    lua_State* State) {
    const Sint32 UniformIndex =
        static_cast<Sint32>(luaL_checkinteger(State, 1));

    switch (lua_type(State, 2)) {
        default:
        TypeError:
            luaL_typeerror(State, 2, "Vector3 or number");
            break;

        case LUA_TNUMBER:
            Game::Graphics::Program::SetUniformFvec3(
                UniformIndex,
                Math::Vector3(static_cast<Math::Vector3::ComponentType>(
                                  lua_tonumber(State, 2)),
                              static_cast<Math::Vector3::ComponentType>(
                                  luaL_checknumber(State, 3)),
                              static_cast<Math::Vector3::ComponentType>(
                                  luaL_checknumber(State, 4))));
            break;

        case LUA_TUSERDATA:
            lua_getmetatable(State, 2);
            luaL_getmetatable(State, "Vector3");
            if (!lua_equal(State, -1, -2)) {
                goto TypeError;
            }

            const Math::Vector3* VecUD =
                static_cast<const Math::Vector3*>(lua_touserdata(State, 2));
            //std::cout << "fvec3 uniform " << VecUD->X << ", " << VecUD->Y << ", " << VecUD->Z << std::endl;

            Game::Graphics::Program::SetUniformFvec3(
                UniformIndex, *VecUD);
            break;
    }

    return 0;
}

int Game::Lua::CLibraries::Graphics::Program::SetUniformIvec4(
    lua_State* State) {
    if (const void* VecUD = LuaHelper::TestMetatable(State, 2, "IVector4");
        VecUD == NULL) {
        Game::Graphics::Program::SetUniformIvec4(
            static_cast<Sint32>(luaL_checkinteger(State, 1)),
            Math::IVector4(static_cast<Sint32>(luaL_checkinteger(State, 2)),
                           static_cast<Sint32>(luaL_checkinteger(State, 3)),
                           static_cast<Sint32>(luaL_checkinteger(State, 4)),
                           static_cast<Sint32>(luaL_checkinteger(State, 5))));
    } else {
        Game::Graphics::Program::SetUniformIvec4(
            static_cast<Sint32>(luaL_checkinteger(State, 1)),
            *static_cast<const Math::IVector4*>(VecUD));
    }
    return 0;
}

int Game::Lua::CLibraries::Graphics::Program::SetUniformUvec4(
    lua_State* State) {
    if (const void* VecUD = LuaHelper::TestMetatable(State, 2, "UVector4");
        VecUD == NULL) {
        Game::Graphics::Program::SetUniformUvec4(
            static_cast<Sint32>(luaL_checkinteger(State, 1)),
            Math::UVector4(static_cast<Uint32>(luaL_checkinteger(State, 2)),
                           static_cast<Uint32>(luaL_checkinteger(State, 3)),
                           static_cast<Uint32>(luaL_checkinteger(State, 4)),
                           static_cast<Uint32>(luaL_checkinteger(State, 5))));
    } else {
        Game::Graphics::Program::SetUniformUvec4(
            static_cast<Sint32>(luaL_checkinteger(State, 1)),
            *static_cast<const Math::UVector4*>(VecUD));
    }
    return 0;
}

int Game::Lua::CLibraries::Graphics::Program::SetUniformBvec4(
    lua_State* State) {
    Game::Graphics::Program::SetUniformBvec4(
        static_cast<Sint32>(luaL_checkinteger(State, 1)),
        Math::IVector4(static_cast<Sint32>(luaL_checkinteger(State, 2)),
                       static_cast<Sint32>(luaL_checkinteger(State, 3)),
                       static_cast<Sint32>(luaL_checkinteger(State, 4)),
                       static_cast<Sint32>(luaL_checkinteger(State, 5))));
    return 0;
}

int Game::Lua::CLibraries::Graphics::Program::SetUniformFvec4(
    lua_State* State) {
    if (const void* VecUD = LuaHelper::TestMetatable(State, 2, "Vector4");
        VecUD == NULL) {
        Game::Graphics::Program::SetUniformFvec4(
            static_cast<Sint32>(luaL_checkinteger(State, 1)),
            Math::Vector4(static_cast<float>(luaL_checknumber(State, 2)),
                          static_cast<float>(luaL_checknumber(State, 3)),
                          static_cast<float>(luaL_checknumber(State, 4)),
                          static_cast<float>(luaL_checknumber(State, 5))));
    } else {
        Game::Graphics::Program::SetUniformFvec4(
            static_cast<Sint32>(luaL_checkinteger(State, 1)),
            *static_cast<const Math::Vector4*>(VecUD));
    }
    return 0;
}

int Game::Lua::CLibraries::Graphics::Program::SetUniformMat2(lua_State* State) {
    Math::Mat2 Mat2;
    const Math::Mat2* Mat2UD;

    if (Mat2UD = static_cast<Math::Mat2*>(
            LuaHelper::TestMetatable(State, 2, "Mat2"));
        Mat2UD == NULL) {
        for (int i = 3; i <= 2 + (2 * 2); ++i) {
            Mat2.IndexElement(i - 3) =
                static_cast<float>(luaL_optnumber(State, i, 0.0));
        }
        Mat2UD = &Mat2;
    }

    Game::Graphics::Program::SetUniformMat2(
        static_cast<Sint32>(luaL_checkinteger(State, 1)), *Mat2UD);
    return 0;
}

int Game::Lua::CLibraries::Graphics::Program::SetUniformMat3(lua_State* State) {
    Math::Mat3 Mat3;
    const Math::Mat3* Mat3UD;

    if (Mat3UD = static_cast<Math::Mat3*>(
            LuaHelper::TestMetatable(State, 2, "Mat3"));
        Mat3UD == NULL) {
        for (int i = 3; i <= 2 + (3 * 3); ++i) {
            Mat3.IndexElement(i - 3) =
                static_cast<float>(luaL_optnumber(State, i, 0.0));
        }
        Mat3UD = &Mat3;
    }

    Game::Graphics::Program::SetUniformMat3(
        static_cast<Sint32>(luaL_checkinteger(State, 1)), *Mat3UD);
    return 0;
}

int Game::Lua::CLibraries::Graphics::Program::SetUniformMat4(lua_State* State) {
    Math::Mat4 Mat4;
    const Math::Mat4* Mat4UD;

    if (Mat4UD = static_cast<Math::Mat4*>(
            LuaHelper::TestMetatable(State, 3, "Mat4"));
        Mat4UD == NULL) {
        for (int i = 3; i <= 2 + (4 * 4); ++i) {
            Mat4.IndexElement(i - 3) =
                static_cast<float>(luaL_optnumber(State, i, 0.0));
        }
        Mat4UD = &Mat4;
    }

    Game::Graphics::Program::SetUniformMat4(
        static_cast<Sint32>(luaL_checkinteger(State, 1)), *Mat4UD);
    return 0;
}

int Game::Lua::CLibraries::Graphics::Classes::Program::AttachShader(
    lua_State* State) {
    const Classes::Shader* ShaderUD =
        static_cast<Classes::Shader*>(LuaHelper::TestMetatable(
            State, 2, CLibraries::Graphics::Shader::MetatableName));
    if (ShaderUD == NULL) {
        luaL_error(
            State,
            "Attempted to attach a non-shader object to a OpenGL program.");
    }

    Game::Graphics::OpenGLFunctions::glAttachShader(
        static_cast<Classes::Program*>(luaL_checkudata(State, 1, "Program"))
            ->GLObject,
        ShaderUD->ShaderObject);
    return 0;
}

int Game::Lua::CLibraries::Graphics::Classes::Program::DetachShader(
    lua_State* State) {
    const Classes::Shader* ShaderUD = static_cast<Classes::Shader*>(
        LuaHelper::TestMetatable(State, 2, "Shader"));
    if (ShaderUD == NULL) {
        luaL_error(
            State,
            "Attempted to detatch a non-shader object from a OpenGL program.");
    }

    Game::Graphics::OpenGLFunctions::glDetachShader(
        static_cast<Classes::Program*>(luaL_checkudata(State, 1, "Program"))
            ->GLObject,
        ShaderUD->ShaderObject);
    return 0;
}

int Game::Lua::CLibraries::Graphics::Classes::Program::DetachAllShaders(
    lua_State* State) {
    GLuint* AttachedShaders;
    GLint AttachedShaderCount;
    GLuint ProgramObject;

    ProgramObject =
        static_cast<Classes::Program*>(luaL_checkudata(State, 1, "Program"))
            ->GLObject;

    Game::Graphics::OpenGLFunctions::glGetProgramiv(
        ProgramObject, GL_ATTACHED_SHADERS, &AttachedShaderCount);

    AttachedShaders = new GLuint[AttachedShaderCount];
    Game::Graphics::OpenGLFunctions::glGetAttachedShaders(
        ProgramObject, AttachedShaderCount, NULL, AttachedShaders);

    for (; AttachedShaderCount > 0; --AttachedShaderCount) {
        Game::Graphics::OpenGLFunctions::glDetachShader(
            ProgramObject, AttachedShaders[AttachedShaderCount - 1]);
    }
    return 0;
}

int Game::Lua::CLibraries::Graphics::Classes::Program::Link(lua_State* State) {
    const Classes::Program* ProgramUD =
        static_cast<Classes::Program*>(luaL_checkudata(State, 1, "Program"));
    Game::Graphics::OpenGLFunctions::glLinkProgram(ProgramUD->GLObject);
    return 0;
}

int Game::Lua::CLibraries::Graphics::Classes::Program::Use(lua_State* State) {
    Game::Graphics::OpenGLFunctions::glUseProgram(
        static_cast<Classes::Program*>(
            LuaHelper::CheckMetatable(State, 1, lua_upvalueindex(1)))
            ->GLObject);
    return 0;
}

int Game::Lua::CLibraries::Graphics::Classes::Program::GetUniformIndex(
    lua_State* State) {
    lua_pushinteger(State,
                    static_cast<lua_Integer>(
                        Game::Graphics::OpenGLFunctions::glGetUniformLocation(
                            static_cast<Classes::Program*>(
                                luaL_checkudata(State, 1, "Program"))
                                ->GLObject,
                            luaL_checkstring(State, 2))));
    return 1;
}

int Game::Lua::CLibraries::Graphics::Classes::Program::GetUniformBlockIndex(
    lua_State* State) {
    lua_pushinteger(State,
                    static_cast<lua_Integer>(
                        Game::Graphics::OpenGLFunctions::glGetUniformBlockIndex(
                            static_cast<Classes::Program*>(
                                luaL_checkudata(State, 1, "Program"))
                                ->GLObject,
                            luaL_checkstring(State, 2))));
    return 1;
}

int Game::Lua::CLibraries::Graphics::Classes::Program::SetUniformBlockBinding(
    lua_State* State) {
    Game::Graphics::OpenGLFunctions::glUniformBlockBinding(
        static_cast<Classes::Program*>(luaL_checkudata(State, 1, "Program"))
            ->GLObject,
        luaL_checkinteger(State, 2),  // UniformBlock Index
        luaL_checkinteger(State, 3)   // Bind To Index
    );
    return 0;
}

int Game::Lua::CLibraries::Graphics::Classes::Program::__gc(lua_State* State) {
    std::cout << "free program" << std::endl;
    Game::Graphics::OpenGLFunctions::glDeleteProgram(
        static_cast<Classes::Program*>(luaL_checkudata(State, 1, "Program"))
            ->GLObject);
    return 0;
}