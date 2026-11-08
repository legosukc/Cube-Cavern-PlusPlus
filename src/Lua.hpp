#pragma once

#include "define.h"

#include "../include/VM/lua.h"

#include <iostream>

#include <SDL3/SDL_filesystem.h>
#include <SDL3/SDL_stdinc.h>
#include <SDL3/SDL_timer.h>

#ifdef BUILD_CLIENT

#ifdef SDL_PLATFORM_VITA
#include <GLES2/gl2.h>
#include "GLES2_typedefs.h"
#else
#include <SDL3/SDL_opengl.h>
#endif

#endif

#include "FunctionHeaders/Exceptions.hpp"
#include "FunctionHeaders/ConfigHandler.hpp"
#include "FunctionHeaders/LuaHelper.hpp"

#include "Time.hpp"

namespace Game::Lua {

    namespace etc {
        struct _LuaDirectoryLibrary {
            inline _LuaDirectoryLibrary() = default;
            _LuaDirectoryLibrary(const char* _Name, int _ParentTableIDX);

            int TableReferenceIndex, ParentTableIndex;
            std::string Path;

            void Load();

            static int __index(lua_State* State);
            // static int __iter(lua_State* State);
        };

        void _LoadLuaFileInTable(const std::filesystem::directory_entry& Entry,
                                 int TableIDX);

        void _LoadLuaAssetDirectoryInTable(const char* DirectoryPath,
                                           int TableIDX);
    }

    // Contains the correct directory paths depending on the build target
    // (Client or Server)
    namespace LuaDirectories {
#ifdef BUILD_CLIENT
        constexpr char MiscClasses[] = "MiscClassesClient";
        constexpr char GameClasses[] = "GameClassesClient";
        constexpr char BaseClasses[] = "BaseClassesClient";
        constexpr char ItemClasses[] = "ItemClassesClient";
        constexpr char EnemyClasses[] = "EnemyClassesClient";
        constexpr char RenderObjects[] = "RenderObjectsClient";
#else
        constexpr char MiscClasses[] = "MiscClassesServer";
        constexpr char GameClasses[] = "GameClassesServer";
        constexpr char BaseClasses[] = "BaseClassesServer";
        constexpr char ItemClasses[] = "ItemClassesServer";
        constexpr char EnemyClasses[] = "EnemyClassesServer";
        constexpr char RenderObjects[] = "RenderObjectsServer";
#endif
    };

    struct LuaThreadInfo {
        LuaThreadInfo();

#ifdef BUILD_CLIENT
        bool isMainState;
        GLuint BoundVertexArray;
        GLuint BoundVertexBuffer;
        GLuint BoundElementBuffer;
        GLuint BoundUniformBuffer;
        GLint BoundTexture = 0;
        GLuint* ActiveTextures;

        GLuint BoundProgram = 0;

        bool DepthTest, StencilTest;
#endif
    };

    // If true, when lua code is compiled the bytecode will be stored to disk.
    // Improves future loading times at the cost of disk space.
    extern bool CacheLuaBytecode;

    // Controls the max amount of storage space for lua bytecode caching.
    extern Uint16 MaxTotalCacheSizeMB;

    extern lua_State* State;

    extern LuaHelper::StackTableReference GameTable;

    void* LuaAllocationFunc(void* ud, void* ptr, size_t osize, size_t nsize);

    inline void Init();
    inline void Destroy();

    inline void Update();
#ifdef BUILD_CLIENT
    inline void Draw();
#endif
}

// #include "LuaLibraries/LuaFont.hpp"

#include "LuaLibraries/LuaConsole.hpp"
#include "LuaLibraries/LuaEnums.hpp"
#include "LuaLibraries/LuaGraphics.hpp"
#include "LuaLibraries/LuaHostUtil.hpp"
#include "LuaLibraries/LuaInput.hpp"
#include "LuaLibraries/LuaMatrix.hpp"
#include "LuaLibraries/LuaModelLoader.hpp"
#include "LuaLibraries/LuaNetwork.hpp"
#include "LuaLibraries/LuaPatching.hpp"
#include "LuaLibraries/LuaSurface.hpp"
#include "LuaLibraries/LuaTask.hpp"
#include "LuaLibraries/LuaTypeHelper.hpp"
#include "LuaLibraries/LuaVector.hpp"

void Game::Lua::Init() {
    const Uint64 StartMS = SDL_GetTicks();
    std::cout << "Initializing Lua." << std::endl;

    {
        const auto CacheLuaBytecodeValue =
            Game::Config::GlobalConfigJSON["Main"]["Lua"]["CacheLuaBytecode"];
        Game::Lua::CacheLuaBytecode = CacheLuaBytecodeValue.is_boolean() &&
                                      CacheLuaBytecodeValue.get<bool>();
    }

    //::_AllocLogFile.open("alloclog.txt");

    Game::Lua::LuaThreadInfo* StateInfo = new Game::Lua::LuaThreadInfo;

    Game::Lua::State = lua_newstate(LuaAllocationFunc, (void*)StateInfo);
    if (Game::Lua::State == NULL) {
        std::cerr << "Failed to create the Lua state." << std::endl;
        Exceptions::ThrowException<Exceptions::RuntimeError>(
            "Failed to create the Lua state.");
    }

#ifdef BUILD_CLIENT
    StateInfo->isMainState = true;
#endif

    //(*static_cast<Game::Lua::LuaThreadInfo**>(
    // lua_getextraspace(Game::Lua::State))) = StateInfo;

    luaopen_base(Game::Lua::State);

    luaopen_math(Game::Lua::State);
    lua_setglobal(Game::Lua::State, LUA_MATHLIBNAME);

    luaopen_integer(Game::Lua::State);
    lua_setglobal(Game::Lua::State, LUA_INTLIBNAME);

    luaopen_table(Game::Lua::State);
    lua_setglobal(Game::Lua::State, LUA_TABLIBNAME);

    luaopen_string(Game::Lua::State);
    lua_setglobal(Game::Lua::State, LUA_STRLIBNAME);

    luaopen_utf8(Game::Lua::State);
    lua_setglobal(Game::Lua::State, LUA_UTF8LIBNAME);

    luaopen_coroutine(Game::Lua::State);
    lua_setglobal(Game::Lua::State, LUA_COLIBNAME);

    luaopen_buffer(Game::Lua::State);
    lua_setglobal(Game::Lua::State, LUA_BUFFERLIBNAME);

    luaopen_bit32(Game::Lua::State);
    lua_setglobal(Game::Lua::State, LUA_BITLIBNAME);

    luaopen_os(Game::Lua::State);
    lua_setglobal(Game::Lua::State, LUA_OSLIBNAME);

    luaopen_debug(Game::Lua::State);
    lua_setglobal(Game::Lua::State, LUA_DBLIBNAME);

    // lua_pushcfunction(Game::Lua::State, print_stdcout, "print");
    // slua_setglobal(Game::Lua::State, "print");

    // Create global tables
    Game::Lua::GameTable =
        LuaHelper::StackTableReference(Game::Lua::State, 0, 4);

    Game::Lua::GameTable.PushReference(Game::Lua::State);
    lua_setglobal(Game::Lua::State, "Game");

    Game::Lua::CLibraries::Enums::Init(Game::Lua::State);

    Game::Lua::CLibraries::Task::Init(Game::Lua::State);

    Game::Lua::CLibraries::TypeHelper::Init(Game::Lua::State);

    Game::Lua::CLibraries::Vector::Init(Game::Lua::State);
    Game::Lua::CLibraries::Matrix::Init(Game::Lua::State);

    Game::Lua::CLibraries::ModelLoader::Init(Game::Lua::State);

    Game::Lua::CLibraries::Graphics::Init(Game::Lua::State);
    Game::Lua::CLibraries::Input::Init(Game::Lua::State);

    lua_createtable(Game::Lua::State, 0, 4);  // Assets

    etc::_LuaDirectoryLibrary ItemClasses, EnemyClasses, MiscClasses,
        BaseClasses, RenderObjects;

    int ParentTableIDX = lua_gettop(Game::Lua::State);
    lua_pushvalue(Game::Lua::State, ParentTableIDX);
    lua_setglobal(Game::Lua::State, "Assets");

    ItemClasses = etc::_LuaDirectoryLibrary(
        Game::Lua::LuaDirectories::ItemClasses, ParentTableIDX);
    EnemyClasses = etc::_LuaDirectoryLibrary(
        Game::Lua::LuaDirectories::EnemyClasses, ParentTableIDX);
    MiscClasses =
        etc::_LuaDirectoryLibrary("MiscClassesGlobal", ParentTableIDX);
    BaseClasses = etc::_LuaDirectoryLibrary(
        Game::Lua::LuaDirectories::BaseClasses, ParentTableIDX);

    RenderObjects = etc::_LuaDirectoryLibrary(
        Game::Lua::LuaDirectories::RenderObjects, ParentTableIDX);

    MiscClasses.Load();
    Lua::CLibraries::Console::PostMiscClassInit(Game::Lua::State);

    etc::_LoadLuaAssetDirectoryInTable(Game::Lua::LuaDirectories::GameClasses,
                                       Game::Lua::GameTable.GetStackIndex());

    BaseClasses.Load();
    RenderObjects.Load();

    // lua_pushnil(Game::Lua::State);
    // lua_setmetatable(Game::Lua::State,
    // Game::Lua::GameTable.GetStackIndex());

    EnemyClasses.Load();
    ItemClasses.Load();

    std::cout << "Initalized Lua system, took " << SDL_GetTicks() - StartMS
              << " milliseconds." << std::endl;
}

void Game::Lua::Update() {
    Game::Lua::GameTable.SetKey(Game::Lua::State, Game::Time::DeltaTime,
                                "DeltaTime");

#ifndef BUILD_SERVER
    CLibraries::Input::Update(Game::Lua::State);
#endif

    CLibraries::Task::Update(Game::Lua::State);

    lua_pushnil(Game::Lua::State);
    const int StackTop = lua_gettop(Game::Lua::State);
    while (lua_next(Game::Lua::State, Game::Lua::GameTable.GetStackIndex()) !=
           0) {
        // std::cout << lua_tostring(Game::Lua::State, -2) << std::endl;
        if (lua_istable(State, -1)) {
            if (lua_getfield(Game::Lua::State, -1, "Update") == LUA_TFUNCTION) {
                lua_pushvalue(Game::Lua::State, -2);
                LuaHelper::PCallLog(Game::Lua::State, 1, 0);
            }
        }

        lua_settop(Game::Lua::State, StackTop);
    }
    lua_settop(Game::Lua::State, StackTop - 1);
}

#ifdef BUILD_CLIENT
void Game::Lua::Draw() {
    lua_getglobal(Game::Lua::State, "Game");
    lua_pushnil(Game::Lua::State);

    const int StackTop = lua_gettop(Game::Lua::State);
    while (lua_next(Game::Lua::State, -2) != 0) {
        if (lua_istable(State, -1)) {
            if (lua_getfield(Game::Lua::State, -1, "Draw") == LUA_TFUNCTION) {
                lua_pushvalue(Game::Lua::State, -2);
                LuaHelper::PCallLog(Game::Lua::State, 1, 0);
            }
        }

        lua_settop(Game::Lua::State, StackTop);
    }
    lua_settop(Game::Lua::State, StackTop - 2);
}
#endif

void Game::Lua::Destroy() {
    if (Game::Lua::State != NULL) {
        lua_close(Game::Lua::State);
        Game::Lua::State = NULL;
    }
}

// uhh keep this for "ClientDownloadsLuaBytecode" config comment
// If true, sends pre-compiled byte code for the client, no compilation on the
// client needed. If false, sends the entire lua source code from mods for the
// client to compile. Bytecode is often smaller than source code, however, all
// client code will have to be compiled on the server on start-up. After
// compilation, bytecode will be cached, you can configure cache settings in
// "GlobalCfg.json"