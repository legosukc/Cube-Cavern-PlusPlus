#pragma once

#include "../define.h"

extern "C" {
#include "../../include/VM/lua.h"
}

#include <SDL3/SDL_platform_defines.h>

#ifdef SDL_PLATFORM_VITA
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include "../GLES2_typedefs.h"
#else
#include <SDL3/SDL_opengl.h>
#endif

#ifdef BUILD_CLIENT

#include "LuaGraphics_Classes/LuaGraphics_BufferBase.hpp"

#include "LuaGraphics_Classes/LuaGraphics_ElementBuffer.hpp"
#include "LuaGraphics_Classes/LuaGraphics_VertexArray.hpp"
#include "LuaGraphics_Classes/LuaGraphics_VertexBuffer.hpp"

#include "LuaGraphics_Classes/LuaGraphics_UniformBuffer.hpp"

#include "LuaGraphics_Classes/LuaGraphics_Program.hpp"
#include "LuaGraphics_Classes/LuaGraphics_Shader.hpp"

#include "LuaGraphics_Classes/LuaGraphics_Texture.hpp"


#include "LuaGraphics_MiscHeaders/LuaGraphics_SampleModels.hpp"

#endif

#include "../FunctionHeaders/LuaHelper.hpp"
//#include "../Lua.hpp"

#include "../ConsoleVars.hpp"


namespace Game::Lua {
    extern LuaHelper::StackTableReference GameTable;
}

namespace Game::ConsoleVars::ReferenceVars::Lua::Graphics {

    struct ChangeErrorCheckingCVar : Game::ConsoleVars::BaseCVar {
        virtual void SetWithString(const char* NewValue) override;

        using VarType = void;
    };
}

namespace Game::Lua::CLibraries::Graphics {
    namespace etc {

#ifdef BUILD_CLIENT
        int DrawArrays(lua_State* State);
        int DrawElements(lua_State* State);

        int DrawArraysInstanced(lua_State* State);

        int DrawElementsInstanced(lua_State* State);

        int SetClearColor(lua_State* State);

        int Clear(lua_State* State);

        template <GLenum Feature>
        int Enable_feature(lua_State* State);

        template <GLenum Feature>
        int Disable_feature(lua_State* State);

        void _lua_graphics_updateScreenSize(int TableIndex,
                                            const Math::IVector2& Size);

        void _lua_graphics_WindowResizedEvent_updateLuaSize(
            const Math::IVector2& Size);

#endif
    }

    inline void Init(lua_State* State);
}

void Game::Lua::CLibraries::Graphics::Init(lua_State* State) {
    using namespace Game::Lua::CLibraries::Graphics::etc;
    LuaHelper::StackTableReference GraphicsTable, SubtableCache;

    // Graphics

    GraphicsTable = LuaHelper::StackTableReference(State, 0, 16);

#ifdef BUILD_CLIENT
    GraphicsTable.SetKeyClosure(State, DrawArrays, "DrawArrays");
    GraphicsTable.SetKeyClosure(State, DrawElements, "DrawElements");

    GraphicsTable.SetKeyClosure(State, DrawArraysInstanced,
                                "DrawArraysInstanced");
    GraphicsTable.SetKeyClosure(State, DrawElementsInstanced,
                                "DrawElementsInstanced");

    GraphicsTable.SetKeyClosure(State, SetClearColor, "SetClearColor");
    GraphicsTable.SetKeyClosure(State, Clear, "Clear");

    GraphicsTable.SetKeyClosure(State, Enable_feature<GL_DEPTH_TEST>,
                                "EnableDepthTest");
    GraphicsTable.SetKeyClosure(State, Disable_feature<GL_DEPTH_TEST>,
                                "DisableDepthTest");

    GraphicsTable.SetKeyClosure(State, Enable_feature<GL_STENCIL_TEST>,
                                "EnableStencilTest");
    GraphicsTable.SetKeyClosure(State, Disable_feature<GL_STENCIL_TEST>,
                                "DisableStencilTest");

    GraphicsTable.SetKeyClosure(State, Enable_feature<GL_BLEND>,
                                "EnableBlending");
    GraphicsTable.SetKeyClosure(State, Disable_feature<GL_BLEND>,
                                "DisableBlending");
#endif

    // DrawModes
    SubtableCache = LuaHelper::StackTableReference(State, 0, 12);

    SubtableCache.SetKey<lua_Integer>(State, GL_POINTS, "Points");

    SubtableCache.SetKey<lua_Integer>(State, GL_LINE_STRIP, "LineStrip");
    SubtableCache.SetKey<lua_Integer>(State, GL_LINE_LOOP, "LineLoop");
    SubtableCache.SetKey<lua_Integer>(State, GL_LINES, "Lines");

#ifndef SDL_PLATFORM_VITA
    SubtableCache.SetKey<lua_Integer>(State, GL_LINE_STRIP_ADJACENCY,
                                      "LineStripAdjacency");
    SubtableCache.SetKey<lua_Integer>(State, GL_LINES_ADJACENCY,
                                      "LinesAdjacency");
#endif

    SubtableCache.SetKey<lua_Integer>(State, GL_TRIANGLE_STRIP,
                                      "TriangleStrip");
    SubtableCache.SetKey<lua_Integer>(State, GL_TRIANGLE_FAN, "TriangleFan");
    SubtableCache.SetKey<lua_Integer>(State, GL_TRIANGLES, "Triangles");

#ifndef SDL_PLATFORM_VITA
    SubtableCache.SetKey<lua_Integer>(State, GL_TRIANGLE_STRIP_ADJACENCY,
                                      "TriangleStripAdjacency");
    SubtableCache.SetKey<lua_Integer>(State, GL_TRIANGLES_ADJACENCY,
                                      "TrianglesAdjacency");
    SubtableCache.SetKey<lua_Integer>(State, GL_PATCHES, "Patches");
#endif

    lua_setfield(State, GraphicsTable.GetStackIndex(), "DrawModes");

    // ShaderTypes
    SubtableCache = LuaHelper::StackTableReference(State, 0, 6);

#ifndef SDL_PLATFORM_VITA
    SubtableCache.SetKey<lua_Integer>(State, GL_COMPUTE_SHADER,
                                      "ComputeShader");
#endif

    SubtableCache.SetKey<lua_Integer>(State, GL_VERTEX_SHADER, "VertexShader");

#ifndef SDL_PLATFORM_VITA
    SubtableCache.SetKey<lua_Integer>(State, GL_TESS_CONTROL_SHADER,
                                      "TessControlShader");
    SubtableCache.SetKey<lua_Integer>(State, GL_TESS_EVALUATION_SHADER,
                                      "TessEvalShader");
#endif

#ifndef SDL_PLATFORM_VITA
    SubtableCache.SetKey<lua_Integer>(State, GL_GEOMETRY_SHADER,
                                      "GeometryShader");
#endif

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

#ifdef SDL_PLATFORM_VITA
    SubtableCache.SetKey<lua_Integer>(State, GL_UNSIGNED_INT_24_8_OES,
                                      "Int24_8");

    SubtableCache.SetKey<lua_Integer>(State, GL_HALF_FLOAT_OES, "F16");
    SubtableCache.SetKey<lua_Integer>(State, GL_HALF_FLOAT_OES, "HalfFloat");
#else
    SubtableCache.SetKey<lua_Integer>(State, GL_UNSIGNED_INT_24_8, "Int24_8");

    SubtableCache.SetKey<lua_Integer>(State, GL_HALF_FLOAT, "F16");
    SubtableCache.SetKey<lua_Integer>(State, GL_HALF_FLOAT, "HalfFloat");
#endif

    SubtableCache.SetKey<lua_Integer>(State, GL_FLOAT, "F32");
    SubtableCache.SetKey<lua_Integer>(State, GL_FLOAT, "Float");

#ifndef SDL_PLATFORM_VITA
    SubtableCache.SetKey<lua_Integer>(State, GL_DOUBLE, "F64");
    SubtableCache.SetKey<lua_Integer>(State, GL_DOUBLE, "Double");
#endif

    lua_setfield(State, GraphicsTable.GetStackIndex(), "Types");

    // Texture Bindings
    SubtableCache = LuaHelper::StackTableReference(State, 0, 4);

#ifndef SDL_PLATFORM_VITA
    SubtableCache.SetKey<lua_Integer>(State, GL_TEXTURE_1D, "Texture1D");
    SubtableCache.SetKey<lua_Integer>(State, GL_TEXTURE_3D, "Texture3D");
#endif

    SubtableCache.SetKey<lua_Integer>(State, GL_TEXTURE_2D, "Texture2D");

    lua_setfield(State, GraphicsTable.GetStackIndex(), "TexBindings");

    // Texture Internal Formats
    SubtableCache = LuaHelper::StackTableReference(State, 0, 64);

#ifndef SDL_PLATFORM_VITA
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
#endif

    SubtableCache.SetKey<lua_Integer>(State, GL_RGB, "RGB");

#ifdef SDL_PLATFORM_VITA
    SubtableCache.SetKey<lua_Integer>(State, GL_RGB8_OES, "RGB8");
#else
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
#endif

    SubtableCache.SetKey<lua_Integer>(State, GL_RGBA, "RGBA");
    SubtableCache.SetKey<lua_Integer>(State, GL_RGBA4, "RGBA4");
    SubtableCache.SetKey<lua_Integer>(State, GL_RGB5_A1, "RGB5A1");

#ifdef SDL_PLATFORM_VITA
    SubtableCache.SetKey<lua_Integer>(State, GL_RGBA8_OES, "RGBA8");
#else
    SubtableCache.SetKey<lua_Integer>(State, GL_RGBA2, "RGBA2");
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
#endif

    SubtableCache.SetKey<lua_Integer>(State, GL_DEPTH_COMPONENT, "Depth");

#ifdef SDL_PLATFORM_VITA

    SubtableCache.SetKey<lua_Integer>(State, GL_DEPTH_COMPONENT24_OES,
                                      "Depth24");

    SubtableCache.SetKey<lua_Integer>(State, GL_DEPTH_STENCIL_OES,
                                      "DepthStencil");
#else
    SubtableCache.SetKey<lua_Integer>(State, GL_DEPTH_COMPONENT24, "Depth24");
    SubtableCache.SetKey<lua_Integer>(State, GL_DEPTH_STENCIL, "DepthStencil");
#endif

    lua_setfield(State, GraphicsTable.GetStackIndex(), "TexInternalFormats");

    // Texture Formats
    SubtableCache = LuaHelper::StackTableReference(State, 0, 15);

#ifndef SDL_PLATFORM_VITA
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
    SubtableCache.SetKey<lua_Integer>(State, GL_DEPTH_STENCIL, "DepthStencil");

#endif
    SubtableCache.SetKey<lua_Integer>(State, GL_DEPTH_COMPONENT, "Depth");

    lua_setfield(State, GraphicsTable.GetStackIndex(), "TexFormats");

    // Texture Wrapping
    SubtableCache = LuaHelper::StackTableReference(State, 0, 15);

    SubtableCache.SetKey<lua_Integer>(State, GL_REPEAT, "Repeat");
    SubtableCache.SetKey<lua_Integer>(State, GL_MIRRORED_REPEAT,
                                      "MirroredRepeat");

#ifndef SDL_PLATFORM_VITA
    SubtableCache.SetKey<lua_Integer>(State, GL_CLAMP_TO_BORDER,
                                      "ClampToBorder");
#endif
    SubtableCache.SetKey<lua_Integer>(State, GL_CLAMP_TO_EDGE, "ClampToEdge");

    lua_setfield(State, GraphicsTable.GetStackIndex(), "TextureWrapping");

    // Texture Filtering
    SubtableCache = LuaHelper::StackTableReference(State, 0, 15);

    SubtableCache.SetKey<lua_Integer>(State, GL_LINEAR, "Linear");
    SubtableCache.SetKey<lua_Integer>(State, GL_NEAREST, "Nearest");

    SubtableCache.SetKey<lua_Integer>(State, GL_NEAREST_MIPMAP_LINEAR,
                                      "NearestMipmapLinear");
    SubtableCache.SetKey<lua_Integer>(State, GL_NEAREST_MIPMAP_NEAREST,
                                      "NearestMipmapNearest");

    SubtableCache.SetKey<lua_Integer>(State, GL_LINEAR_MIPMAP_LINEAR,
                                      "LinearMipmapLinear");
    SubtableCache.SetKey<lua_Integer>(State, GL_LINEAR_MIPMAP_NEAREST,
                                      "LinearMipmapNearest");

    lua_setfield(State, GraphicsTable.GetStackIndex(), "TextureFiltering");

    CLibraries::Graphics::SampleModels::Init(State, GraphicsTable);

#ifdef BUILD_CLIENT

    Game::Window.WindowResizedEvent.Connect(
        _lua_graphics_WindowResizedEvent_updateLuaSize);

    _lua_graphics_updateScreenSize(GraphicsTable.GetStackIndex(),
                                   Game::Window.GetSize());

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

#ifndef SDL_PLATFORM_VITA
    UniformBuffer::Init<GL_UNIFORM_BUFFER>(State, "UniformBuffer",
                                           GraphicsTable);
#endif

    Program::Init(State, GraphicsTable);
    Shader::Init(State, GraphicsTable);

    Texture::Init(State, GraphicsTable);
#endif

    lua_settop(State, GraphicsTable.GetStackIndex());
    lua_setfield(State, Game::Lua::GameTable.GetStackIndex(), "Graphics");
}