#ifndef GAME_LUA_HPP
#define GAME_LUA_HPP

#include "define.h"

#include "../include/VM/lua.h"

#include <SDL3/SDL_stdinc.h>

#ifdef BUILD_CLIENT

#if !defined(SDL_PLATFORM_VITA) || defined(VITA_USE_PVR)
#include <SDL3/SDL_opengl.h>
#else
#include <vitaGL.h>
#endif

#endif

#include "FunctionHeaders/LuaHelper.hpp"

namespace Game::Lua {

    // Contains the correct directory paths depending on the build target
    // (Client or Server)
    namespace LuaDirectories {
#ifdef BUILD_CLIENT
        inline constexpr const char* MiscClasses = "MiscClassesClient";
        inline constexpr const char* GameClasses = "GameClassesClient";
        inline constexpr const char* BaseClasses = "BaseClassesClient";
        inline constexpr const char* ItemClasses = "ItemClassesClient";
        inline constexpr const char* EnemyClasses = "EnemyClassesClient";
        inline constexpr const char* RenderObjects = "RenderObjectsClient";
#else
        inline constexpr const char* MiscClasses = "MiscClassesServer";
        inline constexpr const char* GameClasses = "GameClassesServer";
        inline constexpr const char* BaseClasses = "BaseClassesServer";
        inline constexpr const char* ItemClasses = "ItemClassesServer";
        inline constexpr const char* EnemyClasses = "EnemyClassesServer";
        inline constexpr const char* RenderObjects = "RenderObjectsServer";
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
    inline bool CacheLuaBytecode = false;

    // Controls the max amount of storage space for lua bytecode caching.
    inline Uint16 MaxTotalCacheSizeMB = 20;

    inline lua_State* State = NULL;

    inline LuaHelper::StackTableReference GameTable;

    void Init();
    void Destroy();

    void Update();
#ifdef BUILD_CLIENT
    void Draw();
#endif
}

// uhh keep this for "ClientDownloadsLuaBytecode" config comment
// If true, sends pre-compiled byte code for the client, no compilation on the
// client needed. If false, sends the entire lua source code from mods for the
// client to compile. Bytecode is often smaller than source code, however, all
// client code will have to be compiled on the server on start-up. After
// compilation, bytecode will be cached, you can configure cache settings in
// "GlobalCfg.json"

#endif