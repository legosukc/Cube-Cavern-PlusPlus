#include "Lua.hpp"

#include "define.h"

#include <filesystem>
#include <iostream>

#include "../include/VM/lua.h"
#include "../include/VM/lualib.h"

#include <SDL3/SDL_filesystem.h>
#include <SDL3/SDL_timer.h>

#ifdef BUILD_CLIENT

#ifdef SDL_PLATFORM_VITA
#include <GLES2/gl2.h>
#include "GLES2_typedefs.h"
#else
#include <SDL3/SDL_opengl.h>
#include <SDL3/SDL_opengl_glext.h>
#include <SDL3/SDL_opengles2_gl2ext.h>
#endif

#include "Client/Graphics_Client.hpp"
#endif

#include "FunctionHeaders/Exceptions.hpp"
#include "FunctionHeaders/LuaHelper.hpp"
#include "FunctionHeaders/StringHelper.hpp"

#include "Statistics.hpp"

// uhh keep this for "ClientDownloadsLuaBytecode" config comment
// If true, sends pre-compiled byte code for the client, no compilation on the
// client needed. If false, sends the entire lua source code from mods for the
// client to compile. Bytecode is often smaller than source code, however, all
// client code will have to be compiled on the server on start-up. After
// compilation, bytecode will be cached, you can configure cache settings in
// "GlobalCfg.json"

namespace Game::Lua {
    bool CacheLuaBytecode = false;

    Uint16 MaxTotalCacheSizeMB = 20;

    lua_State* State = NULL;

    LuaHelper::StackTableReference GameTable;
}

Game::Lua::LuaThreadInfo::LuaThreadInfo() {
#ifdef BUILD_CLIENT
    GLint MaxBoundTextures;
    Game::Graphics::OpenGLFunctions::glGetIntegerv(
        GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &MaxBoundTextures);

    this->ActiveTextures =
        static_cast<GLuint*>(std::calloc(0, MaxBoundTextures * sizeof(GLuint)));

    // this->BoundVertexArray = glGetVertexArrayiv()

    this->DepthTest =
        Game::Graphics::OpenGLFunctions::glIsEnabled(GL_DEPTH_TEST);
    this->StencilTest =
        Game::Graphics::OpenGLFunctions::glIsEnabled(GL_STENCIL_TEST);
#endif
}

void Game::Lua::etc::_LoadLuaFileInTable(
    const std::filesystem::directory_entry& Entry,
    int TableIDX) {
    const int StartingStackIndex = lua_gettop(Game::Lua::State);

    const std::filesystem::path& Path = Entry.path();

    unlikely_branch if (Path.extension() != ".lua") {
        std::clog << Path << " doesn't have a .lua extension, skipping"
                  << std::endl;
        return;
    }

    lua_pushstring(Game::Lua::State, Path.stem().generic_string().c_str());
    // lua_pushvalue(Game::Lua::State, -1);

    unlikely_branch if (lua_rawget(Game::Lua::State, TableIDX) != LUA_TNIL) {
        std::clog << Path << " already has an entry, skipping" << std::endl;
        goto _Exit;
    }
    lua_settop(Game::Lua::State, StartingStackIndex);

    std::cout << "Loading " << Path.stem().generic_string().c_str()
              << std::endl;

    unlikely_branch if (LuaHelper::LoadFileLog(Game::Lua::State,
                                               Path.generic_string().c_str()) !=
                        LUA_OK) {
        std::clog << "Failed to load " << Path << std::endl;
        goto _Exit;
    }

    unlikely_branch if (LuaHelper::PCallLog(Game::Lua::State, 0, 1) != LUA_OK) {
        goto _Exit;
    }

    if (!lua_istable(Game::Lua::State, -1)) {
        std::clog << Path << " returned type '"
                  << luaL_typename(Game::Lua::State, -1)
                  << "' instead of type 'table', is this intended behaviour? "
                     "(Ignore this warning if it is)"
                  << std::endl;
    }

    lua_setfield(Game::Lua::State, TableIDX,
                 Path.stem().generic_string().c_str());

_Exit:
    lua_settop(Game::Lua::State, StartingStackIndex);
}

void Game::Lua::etc::_LoadLuaAssetDirectoryInTable(const char* DirectoryPath,
                                                   int TableIDX) {
    SDL_PathInfo DirectoryInfo;

    unlikely_branch if (!SDL_GetPathInfo(DirectoryPath, &DirectoryInfo)) {
        char* WorkingDir = SDL_GetCurrentDirectory();

        std::string ErrorMsg = StringHelper::Combine(
            "Directory ", DirectoryPath,
            " doesn't exist.\nWorking directory: ", WorkingDir);
        SDL_free(WorkingDir);

        std::cerr << ErrorMsg << std::endl;

        Exceptions::ThrowExceptionExpression<Exceptions::IOError>(ErrorMsg);
    }

    unlikely_branch if (DirectoryInfo.type != SDL_PATHTYPE_DIRECTORY) {
        char* WorkingDir = SDL_GetCurrentDirectory();

        std::string ErrorMsg = StringHelper::Combine(
            "Path ", DirectoryPath,
            " isn't a directory.\nWorking directory: ", WorkingDir);
        SDL_free(WorkingDir);

        std::cerr << ErrorMsg << std::endl;

        Exceptions::ThrowExceptionExpression<Exceptions::IOError>(ErrorMsg);
    }

    SDL_EnumerateDirectory(
        DirectoryPath,
        [](void* Userdata, const char* DirectoryPath,
           const char* FileName) -> SDL_EnumerationResult {
            // std::cout << "dir: " << DirectoryPath << std::endl;
            etc::_LoadLuaFileInTable(std::filesystem::directory_entry(
                                      std::string(DirectoryPath) + FileName),
                                  *(int*)Userdata);
            return SDL_EnumerationResult::SDL_ENUM_CONTINUE;
        },
        &TableIDX);
}

Game::Lua::etc::_LuaDirectoryLibrary::_LuaDirectoryLibrary(const char* _Name,
                                                           int _ParentTableIDX)
    : ParentTableIndex(_ParentTableIDX) {
    lua_createtable(Game::Lua::State, 0, 0);
    this->TableReferenceIndex = lua_gettop(Game::Lua::State);

    lua_pushvalue(Game::Lua::State, this->TableReferenceIndex);
    lua_setfield(Game::Lua::State, _ParentTableIDX, _Name);

    // METATABLE
    lua_createtable(Game::Lua::State, 0, 1);

    this->Path = _Name;
    this->Path.push_back('/');

    lua_pushlstring(Game::Lua::State, this->Path.c_str(),
                    this->Path.size() + 1);

    lua_pushcclosure(Game::Lua::State, _LuaDirectoryLibrary::__index, "__index",
                     1);
    lua_setfield(Game::Lua::State, -2, "__index");

    lua_setmetatable(Game::Lua::State, this->TableReferenceIndex);

    lua_settop(Game::Lua::State, this->TableReferenceIndex);
}

void Game::Lua::etc::_LuaDirectoryLibrary::Load() {
    // lua_pushvalue(Game::Lua::State, this->TableReferenceIndex);
    etc::_LoadLuaAssetDirectoryInTable(this->Path.c_str(),
                                    this->TableReferenceIndex);
    // lua_pop(Game::Lua::State, 1);
}

int Game::Lua::etc::_LuaDirectoryLibrary::__index(lua_State* State) {
    lua_pushvalue(State, 2);

    unlikely_branch if (lua_rawget(State, 1) == LUA_TNIL) {
        std::string Path =
            StringHelper::Combine(lua_tostring(State, lua_upvalueindex(1)),
                                  luaL_checkstring(State, 2));
        Path.push_back('.');

        unlikely_branch if (!SDL_GetPathInfo((Path + "lua").c_str(), NULL)) {
            unlikely_branch if (SDL_GetPathInfo((Path + "luau").c_str(),
                                                NULL)) {
                Path.append("luau");
                std::cout
                    << "Loading '" << Path
                    << "luau', but having a '.luau' extension name is "
                       "advised against for compatability with old ass "
                       "filesystems, as some have a limit of 3 characters for "
                       "the extension. I'm a compatability nut, alright?"
                    << std::endl;
            }
            else {
                luaL_error(State, "Attempted to index a non-existant entry '",
                           lua_tostring(State, 2), "' in table ",
                           lua_tostring(State, lua_upvalueindex(1)));
            }
        }
        else {
            Path.append("lua");
        }

        unlikely_branch
            // if (luaL_loadfile(State, Path.c_str()) != LUA_OK) {
            if (LuaHelper::CompileToLuaFunction(State, Path.c_str()) !=
                LUA_OK) {
            lua_error(State);
        }

        switch (lua_pcall(State, 0, 1, 0)) {
            case LUA_YIELD:
                luaL_error(State, Path.c_str(),
                           " yielded during loading (e.g wait()). that's not "
                           "allowed yo");
            case LUA_ERRRUN:
                luaL_error(
                    State, Path.c_str(),
                    " had an error during loading: ", lua_tostring(State, -1));
            case LUA_ERRMEM:
                luaL_error(State, Path.c_str(),
                           " had an error during loading. Ran out of memory.");
            case LUA_ERRERR:
                luaL_error(
                    State, Path.c_str(),
                    " had an error during error handling (e.g pcall()): ",
                    lua_tostring(State, -1));
            case LUA_BREAK:
                // TODO: handle breakpoint
                break;
        }

        lua_pushvalue(State, 2);
        lua_pushvalue(State, -2);
        lua_rawset(State, 1);
    }

    // lua_insert(State, 1);
    //  lua_rotate(State, 1, 1);
    return 1;
}

void* Game::Lua::LuaAllocationFunc(void* ud,
                                   void* ptr,
                                   size_t osize,
                                   size_t nsize) {
    const Uint64 StartNS = SDL_GetTicksNS();

    if (nsize == 0) {
        if (ptr != NULL) {
            ptr = static_cast<char*>(ptr) - sizeof(size_t);

            Game::Statistics::Memory::UpdateLuaStatsDeallocation(
                static_cast<Uint64>(*static_cast<size_t*>(ptr)));
            std::free(ptr);
        }

        Game::Statistics::Memory::LuaNSSpentOnHeapPerFrame +=
            SDL_GetTicksNS() - StartNS;
        return NULL;
    }

    nsize += sizeof(size_t);
    if (ptr == NULL) {
        ptr = std::malloc(nsize);
    } else {
        ptr = std::realloc(static_cast<char*>(ptr) - sizeof(size_t), nsize);
    }
    *static_cast<size_t*>(ptr) = nsize;

    Game::Statistics::Memory::UpdateLuaStatsAllocation(
        static_cast<Uint64>(nsize));
    Game::Statistics::Memory::LuaNSSpentOnHeapPerFrame +=
        SDL_GetTicksNS() - StartNS;

    return static_cast<char*>(ptr) + sizeof(size_t);
}

/*
static int print_stdcout(lua_State* State) {
    const int StackTop = lua_gettop(State);
    for (int i = 1; i <= StackTop; ++i) {
        std::cout << lua_tostring(State, StackTop) << std::endl;
    }
    return 0;
}
*/