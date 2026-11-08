#include "LuaGraphics_Shader.hpp"

#include <stdlib.h>

#ifdef SDL_PLATFORM_VITA
#include <GLES2/gl2.h>
#include "../../GLES2_typedefs.h"
#else
#include <SDL3/SDL_opengl.h>
#include <SDL3/SDL_opengl_glext.h>
#endif

#include <SDL3/SDL_timer.h>

#include "../../../include/VM/lua.h"
#include "../../../include/VM/lualib.h"

#include "../../FunctionHeaders/LuaHelper.hpp"
#include "../../Statistics.hpp"

#include "../../Client/GraphicsClasses/ShaderBase.hpp"

int Game::Lua::CLibraries::Graphics::Shader::__new(lua_State* State) {
    lua_settop(State, 1);

    Classes::Shader* const ShaderUD = static_cast<Classes::Shader*>(
        lua_newuserdata(State, sizeof(Classes::Shader)));
    ShaderUD->ShaderType = static_cast<GLenum>(luaL_checkinteger(State, 1));
    ShaderUD->ShaderObject =
        Game::Graphics::OpenGLFunctions::glCreateShader(ShaderUD->ShaderType);

    luaL_getmetatable(State, CLibraries::Graphics::Shader::MetatableName);
    lua_setmetatable(State, -2);

    return 1;
}

int Game::Lua::CLibraries::Graphics::Classes::Shader::SetShaderSource(
    lua_State* State) {
    const char** ShaderSources;
    int StackTop;
    GLuint ShaderObject;

    ShaderObject =
        static_cast<Classes::Shader*>(
            luaL_checkudata(State, 1,
                            CLibraries::Graphics::Shader::MetatableName))
            ->ShaderObject;

    StackTop = lua_gettop(State);
    ShaderSources = new const char*[StackTop - 1];
    for (int i = 2; i <= StackTop; ++i) {
        ShaderSources[i - 2] = lua_tostring(State, i);
    }

    Game::Graphics::OpenGLFunctions::glShaderSource(ShaderObject, StackTop - 1,
                                                    ShaderSources, NULL);
    delete[] ShaderSources;

    return 0;
}

int Game::Lua::CLibraries::Graphics::Classes::Shader::LoadShaderSourceFromFile(
    lua_State* State) {
    size_t ShaderSourceSize;
    char* ShaderSource;

#ifdef SDL_PLATFORM_VITA
    char* CorrectPath =
#else
    ShaderSource =
        (char*)SDL_LoadFile(luaL_checkstring(State, 2), &ShaderSourceSize);
#endif
        if (ShaderSource == NULL) {
        luaL_error(State, "Shader.LoadShaderSourceFromFile error. File '",
                   lua_tostring(State, 2), "' doesn't exist!");
        return 0;
    }

    Classes::Shader* ShaderUD = (Classes::Shader*)LuaHelper::TestMetatable(
        State, 1, CLibraries::Graphics::Shader::MetatableName);
    if (ShaderUD == NULL) {
        SDL_free(ShaderSource);
        luaL_typeerrorL(State, 1, CLibraries::Graphics::Shader::MetatableName);
        return 0;
    }

    Game::Graphics::OpenGLFunctions::glShaderSource(ShaderUD->ShaderObject, 1,
                                                    &ShaderSource, NULL);
    SDL_free(ShaderSource);
    return 0;
}

int Game::Lua::CLibraries::Graphics::Classes::Shader::GetShaderSource(
    lua_State* State) {
    char* ShaderSource;
    GLint ShaderSourceLength;
    GLuint ShaderObject;
    Uint64 StartNS;

    StartNS = SDL_GetTicksNS();

    ShaderObject =
        static_cast<Classes::Shader*>(
            luaL_checkudata(State, 1,
                            CLibraries::Graphics::Shader::MetatableName))
            ->ShaderObject;

    Game::Graphics::OpenGLFunctions::glGetShaderiv(
        ShaderObject, GL_SHADER_SOURCE_LENGTH, &ShaderSourceLength);

    // directly call std::malloc to not update Game::Statistics::Memory stats.
    ShaderSource = new char[ShaderSourceLength];

    ++Game::Statistics::Memory::EngineAllocationsPerFrame;
    Game::Statistics::Memory::EngineAllocationBytesPerFrame +=
        ShaderSourceLength;

    Game::Graphics::OpenGLFunctions::glGetShaderSource(
        ShaderObject, ShaderSourceLength, NULL, ShaderSource);

    lua_pushlstring(State, ShaderSource, ShaderSourceLength);
    delete[] ShaderSource;

    //++Game::Statistics::Memory::EngineDeallocationsPerFrame;
    Game::Statistics::Memory::EngineNSSpentOnHeapPerFrame +=
        SDL_GetTicksNS() - StartNS;

    return 1;
}

int Game::Lua::CLibraries::Graphics::Classes::Shader::Compile(
    lua_State* State) {
    char* InfoLog;
    GLint InfoLogSize, Success;
    GLuint ShaderObject;

    ShaderObject =
        static_cast<Shader*>(
            luaL_checkudata(State, 1,
                            CLibraries::Graphics::Shader::MetatableName))
            ->ShaderObject;
    // lua_settop(State, 0);

    Game::Graphics::OpenGLFunctions::glCompileShader(ShaderObject);

    Game::Graphics::OpenGLFunctions::glGetShaderiv(ShaderObject,
                                                   GL_COMPILE_STATUS, &Success);
    if (!Success) {
        Game::Graphics::OpenGLFunctions::glGetShaderiv(
            ShaderObject, GL_INFO_LOG_LENGTH, &InfoLogSize);

        static const char InfoMessage[] =
            "Failed to compile shader, errors (new line):\n";
        InfoLog = new char[sizeof(InfoMessage) + InfoLogSize - 1];
        memcpy(InfoLog, InfoMessage, sizeof(InfoMessage) - 1);

        ++Game::Statistics::Memory::EngineAllocationsPerFrame;

        Game::Graphics::OpenGLFunctions::glGetShaderInfoLog(
            ShaderObject, InfoLogSize, NULL, InfoLog + sizeof(InfoMessage));

        lua_pushlstring(State, InfoLog, InfoLogSize + sizeof(InfoMessage) - 1);
        delete[] InfoLog;
        lua_error(State);
        return 0;
    }
    lua_pushboolean(State, (int)true);
    return 1;
}

int Game::Lua::CLibraries::Graphics::Classes::Shader::__gc(lua_State* State) {
    std::cout << "free shader" << std::endl;
    Game::Graphics::OpenGLFunctions::glDeleteShader(
        static_cast<Shader*>(
            luaL_checkudata(State, 1,
                            CLibraries::Graphics::Shader::MetatableName))
            ->ShaderObject);
    return 0;
}