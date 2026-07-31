#pragma once

#include "define.h"

#include <lua.hpp>

#include <SDL3/SDL_filesystem.h>
#include <SDL3/SDL_timer.h>

#ifdef BUILD_CLIENT
#include <SDL3/SDL_opengl.h>
#include <SDL3/SDL_opengl_glext.h>
#endif

#include "FunctionHeaders/LuaHelper.hpp"

namespace Game::Lua {

    // Contains the correct directory paths depending on the build target
    // (Client or Server)
    struct LuaDirectories {
#ifdef BUILD_CLIENT
        static inline const char* MiscClasses = "MiscClassesClient";
        static inline const char* GameClasses = "GameClassesClient";
        static inline const char* BaseClasses = "BaseClassesClient";
        static inline const char* ItemClasses = "ItemClassesClient";
        static inline const char* EnemyClasses = "EnemyClassesClient";
#else
        static inline const char* MiscClasses = "MiscClassesServer";
        static inline const char* GameClasses = "GameClassesServer";
        static inline const char* BaseClasses = "BaseClassesServer";
        static inline const char* ItemClasses = "ItemClassesServer";
        static inline const char* EnemyClasses = "EnemyClassesServer";
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

    void* LuaAllocationFunc(void*, void* ptr, size_t, size_t nsize);

    inline void Init();
    inline void Destroy();

    inline void Update();
#ifdef BUILD_CLIENT
    void Draw();
#endif

    extern lua_State* State;

    extern LuaHelper::StackTableReference GameTable;
}

// uhh keep this for "ClientDownloadsLuaBytecode" config comment
// If true, sends pre-compiled byte code for the client, no compilation on the
// client needed. If false, sends the entire lua source code from mods for the
// client to compile. Bytecode is often smaller than source code, however, all
// client code will have to be compiled on the server on start-up. After
// compilation, bytecode will be cached, you can configure cache settings in
// "GlobalCfg.json"