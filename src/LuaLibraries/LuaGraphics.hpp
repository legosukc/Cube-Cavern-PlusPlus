#pragma once

#include "../define.h"

#include "../../include/VM/lua.h"

#ifdef BUILD_CLIENT

#include "LuaGraphics_Classes/LuaGraphics_BufferBase.hpp"

#include "LuaGraphics_Classes/LuaGraphics_ElementBuffer.hpp"
#include "LuaGraphics_Classes/LuaGraphics_VertexArray.hpp"
#include "LuaGraphics_Classes/LuaGraphics_VertexBuffer.hpp"

#include "LuaGraphics_Classes/LuaGraphics_UniformBuffer.hpp"

#include "LuaGraphics_Classes/LuaGraphics_Program.hpp"
#include "LuaGraphics_Classes/LuaGraphics_Shader.hpp"

#include "LuaGraphics_Classes/LuaGraphics_Texture.hpp"

#endif

#include "../ConsoleVars.hpp"

#include "../FunctionHeaders/LuaHelper.hpp"
#include "../Lua.hpp"

namespace Game::ConsoleVars::ReferenceVars::Lua::Graphics {

    struct ChangeErrorCheckingCVar : Game::ConsoleVars::BaseCVar {
        virtual void Set(const char* Argument) override {}

        using VarType = void;
    };
}

namespace Game::Lua::CLibraries::Graphics {
    inline void Init(lua_State* State);

#ifdef BUILD_CLIENT
    static int DrawArrays(lua_State* State);
    static int DrawElements(lua_State* State);

    static int DrawArraysInstanced(lua_State* State);
    static int DrawElementsInstanced(lua_State* State);

    static int SetClearColor(lua_State* State);
    static int Clear(lua_State* State);
#endif
}

#ifdef BUILD_CLIENT
int Game::Lua::CLibraries::Graphics::DrawArrays(lua_State* State) {
    Game::Graphics::OpenGLFunctions::glDrawArrays(
        static_cast<GLenum>(luaL_checkinteger(State, 1)),  // Mode
        static_cast<GLint>(luaL_optinteger(State, 2, 0)),  // First
        static_cast<GLsizei>(luaL_checkinteger(State, 3))  // Count
    );
    return 0;
}

int Game::Lua::CLibraries::Graphics::DrawElements(lua_State* State) {
    Game::Graphics::OpenGLFunctions::glDrawElements(
        static_cast<GLenum>(luaL_checkinteger(State, 1)),   // Mode
        static_cast<GLsizei>(luaL_checkinteger(State, 2)),  // Count
        static_cast<GLenum>(luaL_checkinteger(State, 3)),   // Type
        reinterpret_cast<const void*>(                      // Offset
            luaL_optinteger(State, 4, 0)));
    return 0;
}

int Game::Lua::CLibraries::Graphics::DrawArraysInstanced(lua_State* State) {
    Game::Graphics::OpenGLFunctions::glDrawArraysInstanced(
        static_cast<GLenum>(luaL_checkinteger(State, 1)),   // Mode
        static_cast<GLint>(luaL_optinteger(State, 2, 0)),   // First
        static_cast<GLsizei>(luaL_checkinteger(State, 3)),  // Count
        static_cast<GLsizei>(luaL_checkinteger(State, 4))   // Instance Count
    );
    return 0;
}

int Game::Lua::CLibraries::Graphics::DrawElementsInstanced(lua_State* State) {
    Game::Graphics::OpenGLFunctions::glDrawElementsInstanced(
        static_cast<GLenum>(luaL_checkinteger(State, 1)),   // Mode
        static_cast<GLsizei>(luaL_checkinteger(State, 2)),  // Count
        static_cast<GLenum>(luaL_checkinteger(State, 3)),   // Type
        reinterpret_cast<const void*>(                      // Offset
            luaL_optinteger(State, 4, 0)),
        static_cast<GLsizei>(luaL_checkinteger(State, 5))  // Instance Count
    );
    return 0;
}

int Game::Lua::CLibraries::Graphics::SetClearColor(lua_State* State) {
    Game::Graphics::SetClearColor(
        static_cast<float>(luaL_optnumber(State, 1, 0.0)),
        static_cast<float>(luaL_optnumber(State, 2, 0.0)),
        static_cast<float>(luaL_optnumber(State, 3, 0.0)),
        static_cast<float>(luaL_optnumber(State, 4, 0.0)));
    return 0;
}

int Game::Lua::CLibraries::Graphics::Clear(lua_State* State) {
    Game::Graphics::ClearBitfields(Game::Graphics::BufferBitfields.ColorBit |
                                   Game::Graphics::BufferBitfields.DepthBit |
                                   Game::Graphics::BufferBitfields.StencilBit);
    return 0;
}
#endif

#ifdef BUILD_CLIENT
namespace {

    static void _lua_graphics_updateScreenSize(int TableIndex,
                                               const Math::IVector2& Size) {
        lua_pushinteger(Game::Lua::State, Size.X);
        lua_setfield(Game::Lua::State, TableIndex, "ResolutionX");

        lua_pushinteger(Game::Lua::State, Size.Y);
        lua_setfield(Game::Lua::State, TableIndex, "ResolutionY");

        lua_pushnumber(Game::Lua::State, static_cast<double>(Size.X) /
                                             static_cast<double>(Size.Y));
        lua_setfield(Game::Lua::State, TableIndex, "AspectRation");
    }

    static void _lua_graphics_WindowResizedEvent_updateLuaSize(
        const Math::IVector2& Size) {
        Game::Lua::GameTable.PushKey(Game::Lua::State, "Graphics");

        _lua_graphics_updateScreenSize(-2, Size);

        lua_pop(Game::Lua::State, 1);
    }
}
#endif

void Game::Lua::CLibraries::Graphics::Init(lua_State* State) {
    LuaHelper::StackTableReference GraphicsTable, SubtableCache;

    // Graphics

    GraphicsTable = LuaHelper::StackTableReference(State, 0, 16);

#ifdef BUILD_CLIENT
    GraphicsTable.SetKeyClosure(State, Graphics::DrawArrays, "DrawArrays");
    GraphicsTable.SetKeyClosure(State, Graphics::DrawElements, "DrawElements");

    GraphicsTable.SetKeyClosure(State, Graphics::DrawArraysInstanced,
                                "DrawArraysInstanced");
    GraphicsTable.SetKeyClosure(State, Graphics::DrawElementsInstanced,
                                "DrawElementsInstanced");

    GraphicsTable.SetKeyClosure(State, Graphics::SetClearColor,
                                "SetClearColor");
    GraphicsTable.SetKeyClosure(State, Graphics::Clear, "Clear");
#endif

    // DrawModes
    SubtableCache = LuaHelper::StackTableReference(State, 0, 12);

    SubtableCache.SetKey<lua_Integer>(State, GL_POINTS, "Points");

    SubtableCache.SetKey<lua_Integer>(State, GL_LINE_STRIP, "LineStrip");
    SubtableCache.SetKey<lua_Integer>(State, GL_LINE_LOOP, "LineLoop");
    SubtableCache.SetKey<lua_Integer>(State, GL_LINES, "Lines");
    SubtableCache.SetKey<lua_Integer>(State, GL_LINE_STRIP_ADJACENCY,
                                      "LineStripAdjacency");
    SubtableCache.SetKey<lua_Integer>(State, GL_LINES_ADJACENCY,
                                      "LinesAdjacency");

    SubtableCache.SetKey<lua_Integer>(State, GL_TRIANGLE_STRIP,
                                      "TriangleStrip");
    SubtableCache.SetKey<lua_Integer>(State, GL_TRIANGLE_FAN, "TriangleFan");
    SubtableCache.SetKey<lua_Integer>(State, GL_TRIANGLES, "Triangles");
    SubtableCache.SetKey<lua_Integer>(State, GL_TRIANGLE_STRIP_ADJACENCY,
                                      "TriangleStripAdjacency");
    SubtableCache.SetKey<lua_Integer>(State, GL_TRIANGLES_ADJACENCY,
                                      "TrianglesAdjacency");

    SubtableCache.SetKey<lua_Integer>(State, GL_PATCHES, "Patches");

    lua_setfield(State, GraphicsTable.GetStackIndex(), "DrawModes");

    // ShaderTypes
    SubtableCache = LuaHelper::StackTableReference(State, 0, 6);

    SubtableCache.SetKey<lua_Integer>(State, GL_COMPUTE_SHADER,
                                      "ComputeShader");

    SubtableCache.SetKey<lua_Integer>(State, GL_VERTEX_SHADER, "VertexShader");

    SubtableCache.SetKey<lua_Integer>(State, GL_TESS_CONTROL_SHADER,
                                      "TessControlShader");
    SubtableCache.SetKey<lua_Integer>(State, GL_TESS_EVALUATION_SHADER,
                                      "TessEvalShader");

    SubtableCache.SetKey<lua_Integer>(State, GL_GEOMETRY_SHADER,
                                      "GeometryShader");
    SubtableCache.SetKey<lua_Integer>(State, GL_FRAGMENT_SHADER,
                                      "FragmentShader");

    lua_setfield(State, GraphicsTable.GetStackIndex(), "ShaderTypes");

    // Types
    SubtableCache = LuaHelper::StackTableReference(State, 0, 10);

    SubtableCache.SetKey<lua_Integer>(State, GL_UNSIGNED_BYTE, "Uint8");
    SubtableCache.SetKey<lua_Integer>(State, GL_UNSIGNED_SHORT, "Uint16");
    SubtableCache.SetKey<lua_Integer>(State, GL_UNSIGNED_INT, "Uint32");

    SubtableCache.SetKey<lua_Integer>(State, GL_BYTE, "Int8");
    SubtableCache.SetKey<lua_Integer>(State, GL_SHORT, "Int16");
    SubtableCache.SetKey<lua_Integer>(State, GL_INT, "Int32");

    SubtableCache.SetKey<lua_Integer>(State, GL_HALF_FLOAT, "F16");
    SubtableCache.SetKey<lua_Integer>(State, GL_HALF_FLOAT, "HalfFloat");

    SubtableCache.SetKey<lua_Integer>(State, GL_FLOAT, "F32");
    SubtableCache.SetKey<lua_Integer>(State, GL_FLOAT, "Float");

    SubtableCache.SetKey<lua_Integer>(State, GL_DOUBLE, "F64");
    SubtableCache.SetKey<lua_Integer>(State, GL_DOUBLE, "Double");

    lua_setfield(State, GraphicsTable.GetStackIndex(), "Types");

    // Texture Bindings
    SubtableCache = LuaHelper::StackTableReference(State, 0, 4);

    SubtableCache.SetKey<lua_Integer>(State, GL_TEXTURE_1D, "Texture1D");
    SubtableCache.SetKey<lua_Integer>(State, GL_TEXTURE_2D, "Texture2D");
    SubtableCache.SetKey<lua_Integer>(State, GL_TEXTURE_3D, "Texture3D");
    // SubtableCache.SetKey<lua_Integer>(State, GL_TEXTURE_3D, "Texture3D");

    lua_setfield(State, GraphicsTable.GetStackIndex(), "TexBindings");

    // Texture Internal Formats
    SubtableCache = LuaHelper::StackTableReference(State, 0, 64);

    SubtableCache.SetKey<lua_Integer>(State, GL_RED, "Red");
    SubtableCache.SetKey<lua_Integer>(State, GL_R8, "Red8");
    SubtableCache.SetKey<lua_Integer>(State, GL_R8I, "Red8I");
    SubtableCache.SetKey<lua_Integer>(State, GL_R8UI, "Red8UI");
    SubtableCache.SetKey<lua_Integer>(State, GL_R8_SNORM, "Red8sNorm");
    SubtableCache.SetKey<lua_Integer>(State, GL_R16, "Red16");
    SubtableCache.SetKey<lua_Integer>(State, GL_R16I, "Red16I");
    SubtableCache.SetKey<lua_Integer>(State, GL_R16UI, "Red16UI");
    SubtableCache.SetKey<lua_Integer>(State, GL_R16F, "Red16F");
    SubtableCache.SetKey<lua_Integer>(State, GL_R16_SNORM, "Red16sNorm");
    SubtableCache.SetKey<lua_Integer>(State, GL_R32I, "Red32I");
    SubtableCache.SetKey<lua_Integer>(State, GL_R32UI, "Red32UI");
    SubtableCache.SetKey<lua_Integer>(State, GL_R32F, "Red32F");

    SubtableCache.SetKey<lua_Integer>(State, GL_RG, "RG");
    SubtableCache.SetKey<lua_Integer>(State, GL_RG8, "RG8");
    SubtableCache.SetKey<lua_Integer>(State, GL_RG8I, "RG8I");
    SubtableCache.SetKey<lua_Integer>(State, GL_RG8UI, "RG8UI");
    SubtableCache.SetKey<lua_Integer>(State, GL_RG8_SNORM, "RG8sNorm");
    SubtableCache.SetKey<lua_Integer>(State, GL_RG16, "RG16");
    SubtableCache.SetKey<lua_Integer>(State, GL_RG16I, "RG16I");
    SubtableCache.SetKey<lua_Integer>(State, GL_RG16UI, "RG16UI");
    SubtableCache.SetKey<lua_Integer>(State, GL_RG16F, "RG16F");
    SubtableCache.SetKey<lua_Integer>(State, GL_RG16_SNORM, "RG16sNorm");
    SubtableCache.SetKey<lua_Integer>(State, GL_RG32I, "RG32I");
    SubtableCache.SetKey<lua_Integer>(State, GL_RG32UI, "RG32UI");
    SubtableCache.SetKey<lua_Integer>(State, GL_RG32F, "RG32F");

    SubtableCache.SetKey<lua_Integer>(State, GL_RGB, "RGB");
    SubtableCache.SetKey<lua_Integer>(State, GL_RGB4, "RGB4");
    SubtableCache.SetKey<lua_Integer>(State, GL_RGB5, "RGB5");
    SubtableCache.SetKey<lua_Integer>(State, GL_RGB8, "RGB8");
    SubtableCache.SetKey<lua_Integer>(State, GL_RGB8I, "RGB8I");
    SubtableCache.SetKey<lua_Integer>(State, GL_RGB8UI, "RGB8UI");
    SubtableCache.SetKey<lua_Integer>(State, GL_RGB10, "RGB10");
    SubtableCache.SetKey<lua_Integer>(State, GL_RGB12, "RGB12");
    SubtableCache.SetKey<lua_Integer>(State, GL_RGB16, "RGB16");
    SubtableCache.SetKey<lua_Integer>(State, GL_RGB16I, "RGB16I");
    SubtableCache.SetKey<lua_Integer>(State, GL_RGB16UI, "RGB16UI");
    SubtableCache.SetKey<lua_Integer>(State, GL_RGB16F, "RGB16F");
    SubtableCache.SetKey<lua_Integer>(State, GL_RGB16_SNORM, "RGB16sNorm");
    SubtableCache.SetKey<lua_Integer>(State, GL_RGB9_E5, "RGB9E5");
    SubtableCache.SetKey<lua_Integer>(State, GL_R11F_G11F_B10F, "R11FG11FB10F");
    SubtableCache.SetKey<lua_Integer>(State, GL_RGB32I, "RGB32I");
    SubtableCache.SetKey<lua_Integer>(State, GL_RGB32UI, "RGB32UI");
    SubtableCache.SetKey<lua_Integer>(State, GL_RGB32F, "RGB32F");

    SubtableCache.SetKey<lua_Integer>(State, GL_SRGB8, "SRGB8");

    SubtableCache.SetKey<lua_Integer>(State, GL_RGBA, "RGBA");
    SubtableCache.SetKey<lua_Integer>(State, GL_RGBA2, "RGBA2");
    SubtableCache.SetKey<lua_Integer>(State, GL_RGBA4, "RGBA4");
    SubtableCache.SetKey<lua_Integer>(State, GL_RGB5_A1, "RGB5A1");
    SubtableCache.SetKey<lua_Integer>(State, GL_RGBA8, "RGBA8");
    SubtableCache.SetKey<lua_Integer>(State, GL_RGBA8I, "RGBA8I");
    SubtableCache.SetKey<lua_Integer>(State, GL_RGBA8UI, "RGBA8UI");
    SubtableCache.SetKey<lua_Integer>(State, GL_RGBA8_SNORM, "RGBA8sNorm");
    SubtableCache.SetKey<lua_Integer>(State, GL_RGB10_A2, "RGB10_A2");
    SubtableCache.SetKey<lua_Integer>(State, GL_RGB10_A2UI, "RGB10_A2UI");
    SubtableCache.SetKey<lua_Integer>(State, GL_RGBA12, "RGBA12");
    SubtableCache.SetKey<lua_Integer>(State, GL_RGBA16, "RGBA16");
    SubtableCache.SetKey<lua_Integer>(State, GL_RGBA16I, "RGBA16I");
    SubtableCache.SetKey<lua_Integer>(State, GL_RGBA16UI, "RGBA16UI");
    SubtableCache.SetKey<lua_Integer>(State, GL_RGBA16F, "RGBA16F");
    SubtableCache.SetKey<lua_Integer>(State, GL_RGBA32I, "RGBA32I");
    SubtableCache.SetKey<lua_Integer>(State, GL_RGBA32UI, "RGBA32UI");
    SubtableCache.SetKey<lua_Integer>(State, GL_RGBA32F, "RGBA32F");
    SubtableCache.SetKey<lua_Integer>(State, GL_SRGB8_ALPHA8, "SRGB8A8");

    SubtableCache.SetKey<lua_Integer>(State, GL_DEPTH_COMPONENT, "Depth");
    SubtableCache.SetKey<lua_Integer>(State, GL_DEPTH_STENCIL, "DepthStencil");

    lua_setfield(State, GraphicsTable.GetStackIndex(), "TexInternalFormats");

    // Texture Formats
    SubtableCache = LuaHelper::StackTableReference(State, 0, 15);

    SubtableCache.SetKey<lua_Integer>(State, GL_RED, "Red");
    SubtableCache.SetKey<lua_Integer>(State, GL_RED_INTEGER, "RedInt");

    SubtableCache.SetKey<lua_Integer>(State, GL_RG, "RG");
    SubtableCache.SetKey<lua_Integer>(State, GL_RG_INTEGER, "RGInt");

    SubtableCache.SetKey<lua_Integer>(State, GL_RGB, "RGB");
    SubtableCache.SetKey<lua_Integer>(State, GL_RGB_INTEGER, "RGBInt");

    SubtableCache.SetKey<lua_Integer>(State, GL_BGR, "BGR");
    SubtableCache.SetKey<lua_Integer>(State, GL_BGR_INTEGER, "BGRInt");

    SubtableCache.SetKey<lua_Integer>(State, GL_RGBA, "RGBA");
    SubtableCache.SetKey<lua_Integer>(State, GL_RGBA_INTEGER, "RGBAInt");

    SubtableCache.SetKey<lua_Integer>(State, GL_BGRA, "BGRA");
    SubtableCache.SetKey<lua_Integer>(State, GL_BGRA_INTEGER, "BGRAInt");

    SubtableCache.SetKey<lua_Integer>(State, GL_STENCIL_INDEX, "Stencil");
    SubtableCache.SetKey<lua_Integer>(State, GL_DEPTH_COMPONENT, "Depth");
    SubtableCache.SetKey<lua_Integer>(State, GL_DEPTH_STENCIL, "DepthStencil");

    lua_setfield(State, GraphicsTable.GetStackIndex(), "TexFormats");

#ifdef BUILD_CLIENT

    Game::Window.WindowResizedEvent.Connect(
        ::_lua_graphics_WindowResizedEvent_updateLuaSize);

    ::_lua_graphics_updateScreenSize(GraphicsTable.GetStackIndex(),
                                     Game::Window.Size);

    lua_createtable(State, 0, 4);

    SubtableCache = LuaHelper::StackTableReference(State, 0, 3);

    SubtableCache.SetKey<lua_Integer>(State, SDL_GL_CONTEXT_PROFILE_CORE,
                                      "Core");
    SubtableCache.SetKey<lua_Integer>(
        State, SDL_GL_CONTEXT_PROFILE_COMPATIBILITY, "Compatability");
    SubtableCache.SetKey<lua_Integer>(State, SDL_GL_CONTEXT_PROFILE_ES, "ES");

    lua_setfield(State, -2, "ProfileEnums");

    lua_pushinteger(State, Game::Graphics::OpenGL::MajorVersion);
    lua_setfield(State, -2, "MajorVersion");

    lua_pushinteger(State, Game::Graphics::OpenGL::MinorVersion);
    lua_setfield(State, -2, "MinorVersion");

    lua_pushinteger(State,
                    static_cast<lua_Integer>(Game::Graphics::OpenGL::Profile));
    lua_setfield(State, -2, "Profile");

    lua_setfield(State, GraphicsTable.GetStackIndex(), "OpenGL");

    Classes::VertexBuffer::InitMetatable(State, "VertexBuffer");
    Classes::ElementBuffer::InitMetatable(State, "ElementBuffer");

    Classes::UniformBuffer::InitMetatable(State);

    VertexArray::Init(State, GraphicsTable);
    VertexBuffer::Init<GL_ARRAY_BUFFER>(State, "VertexBuffer", GraphicsTable);
    ElementBuffer::Init<GL_ELEMENT_ARRAY_BUFFER>(State, "ElementBuffer",
                                                 GraphicsTable);

    UniformBuffer::Init<GL_UNIFORM_BUFFER>(State, "UniformBuffer",
                                           GraphicsTable);

    Program::Init(State, GraphicsTable);
    Shader::Init(State, GraphicsTable);

    Texture::Init(State, GraphicsTable);
#endif

    lua_settop(State, GraphicsTable.GetStackIndex());
    lua_setfield(State, Game::Lua::GameTable.GetStackIndex(), "Graphics");
}